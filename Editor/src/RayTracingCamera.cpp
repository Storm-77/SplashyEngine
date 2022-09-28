#include "RayTracingCamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Input/Input.hpp>
#include <imgui.h>

namespace Sandbox
{
    void RayTracingCamera::OnUpdate(ant::TimeStep ts)
    {
        glm::ivec2 mouseCords = ant::Input::MousePos();
        glm::ivec2 mouseDelta = m_mousePrev - mouseCords;
        m_mousePrev = mouseCords;
        mouseDelta.x = -mouseDelta.x;

        float t = ts.Seconds();
        glm::vec3 rightDirection = glm::cross(m_forwardDirection, m_upDirection);

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_W))
        {
            m_position += m_forwardDirection * m_movementSpeed * t;
            m_moved = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_S))
        {
            m_position -= m_forwardDirection * m_movementSpeed * t;
            m_moved = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_D))
        {
            m_position += rightDirection * m_movementSpeed * t;
            m_moved = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_A))
        {
            m_position -= rightDirection * m_movementSpeed * t;
            m_moved = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_SPACE))
        {
            m_position += m_upDirection * m_movementSpeed * t;
            m_moved = true;
        }
        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_SHIFT))
        {
            m_position -= m_upDirection * m_movementSpeed * t;
            m_moved = true;
        }

        // camera rotation

        if (m_rotated && !ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_CONTROL))
        {
            ant::Input::SetCursor(ant::CursorStyle::Normal);
            m_rotated = false;
        }

        if (ant::Input::IsKeyPressed(ant::KeyCode::KEY_LEFT_CONTROL) && (mouseDelta.x != 0.f || mouseDelta.y != 0.f))
        {
            ant::Input::SetCursor(ant::CursorStyle::Disabled);

            float yawDelta = mouseDelta.x * m_mouseSpeed;
            float pitchDelta = mouseDelta.y * m_mouseSpeed;

            glm::quat q = glm::normalize(

                glm::cross(
                    glm::angleAxis(-pitchDelta, rightDirection),
                    glm::angleAxis(-yawDelta, m_upDirection))

            );
            m_forwardDirection = glm::rotate(q, m_forwardDirection);

            m_rotated = true;
        }

        if (m_rotated || m_moved || m_resized) // apply transformations
        {
            CalculateView();       // moved, rotated
            CalculateProjection(); // resize
            CalculateRays();
            m_moved = m_resized = false;
        }
    }

    void RayTracingCamera::CalculateProjection()
    {
        m_projection = glm::perspectiveFov(
            glm::radians(m_verticalFOV),
            (float)m_viewportWidth,
            (float)m_viewportHeight,
            m_nearClip,
            m_farClip);

        m_inverseProjection = glm::inverse(m_projection);
    }

    void RayTracingCamera::CalculateView()
    {
        auto focusPoint = m_position + m_forwardDirection;
        m_view = glm::lookAt(m_position, focusPoint, m_upDirection);
        m_inverseView = glm::inverse(m_view);
    }

    void RayTracingCamera::CalculateRays()
    {
        m_rayDirections.resize(m_viewportHeight * m_viewportWidth);

        for (size_t y = 0; y < m_viewportHeight; y++)
        {
            for (size_t x = 0; x < m_viewportWidth; x++)
            {
                glm::vec2 coord = {float(x) / float(m_viewportWidth), float(y) / float(m_viewportHeight)};
                coord = coord * 2.f - 1.f;

                glm::vec4 target = m_inverseProjection * glm::vec4(coord, 1, 1); // maps pixel coordinates to camera coordinate system
                auto direction = m_inverseView * glm::vec4(
                                                     glm::normalize(glm::vec3(target) / target.w), // convert to non homogeneous 3d coordinate system and normalize to get directional vector
                                                     0);

                m_rayDirections[x + y * m_viewportWidth] = direction; // direction in world space
            }
        }
    }

}