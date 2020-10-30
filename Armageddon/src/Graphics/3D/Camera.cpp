#include "Camera.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "../../Log.h"
Camera::Camera()
{
    this->pos = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->rotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);
    UpdateViewMatrix();
}

void Camera::SetLookAtPos(DirectX::XMFLOAT3 LookPos)
{
    // position ne peut pas être la même que la position de la camera 
    if (LookPos.x == this->pos.x && LookPos.y == this->pos.y && LookPos.z == this->pos.z)
    {
        return;
    }

    LookPos.x = this->pos.x - LookPos.x;
    LookPos.y = this->pos.z - LookPos.y;
    LookPos.z = this->pos.y - LookPos.y;

    float pitch = 0.0f;
    if (LookPos.y != 0.0f) // calcule l'amplitude de l'angle de la caméra dans l'axe Y (pythagore) 
    {
        const float Dist = sqrt(LookPos.x * LookPos.x + LookPos.z * LookPos.z);
        pitch = atan(LookPos.y / Dist);
            
    }

    this->SetRotation(pitch, 0.0f, 0.0f);
}

void Camera::SetZoomLevel(float windowWidth, float windowHeight, float ZoomLevel)
{

    //this->ProjectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(-aspectRatio * ZoomLevel, aspectRatio * ZoomLevel,-1.0f * ZoomLevel,1.0f * ZoomLevel, -0.1f, 10.0f);
   float aspectRatio = windowWidth / windowHeight ;
    float fovRad = (90.0f * ZoomLevel / 360.0f) * DirectX::XM_2PI;
    this->ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio , 0.1f, 1000.0f);

}

const DirectX::XMVECTOR& Camera::GetForwardVec()
{
    return this->vec_forward;
}

const DirectX::XMVECTOR& Camera::GetBackwardVec()
{
    return this->vec_backward;
}

const DirectX::XMVECTOR& Camera::GetLeftVec()
{
    return this->vec_left;
}

const DirectX::XMVECTOR& Camera::GetRightVec()
{
    return this->vec_right;
}

const DirectX::XMVECTOR& Camera::GetUpVec()
{
    return vec_up;
}

const DirectX::XMVECTOR& Camera::GetDownVec()
{
    return this->vec_down;
}

void Camera::SetProjectionValues(float viewWidth, float viewHeight, float nearZ, float FarZ)
{
    float fovRad = (90.0f / 360.0f) * DirectX::XM_2PI;
  float aspectRatio =  viewWidth / viewHeight;
   // this->ProjectionMatrix = DirectX::XMMatrixOrthographicOffCenterLH(-aspectRatio, aspectRatio, -1.0f, 1.0f, nearZ, FarZ);
    this->ProjectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, FarZ);
    
}



void Camera::SetPosition(const DirectX::XMVECTOR& pos)
{
    DirectX::XMStoreFloat3(&this->pos,pos);
    this->posVector = pos;
    this->UpdateViewMatrix();
}

void Camera::SetPosition(float x, float y, float z)
{
   this->pos = DirectX::XMFLOAT3(x,y,z);
   this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}



void Camera::AdjustPosition(const DirectX::XMVECTOR& pos)
{
    this->posVector += pos;
    DirectX::XMStoreFloat3(&this->pos, this->posVector);
    UpdateViewMatrix();
}

void Camera::AdjustPosition(float x, float y, float z)
{
    this->pos.x += x;
    this->pos.y += y;
    this->pos.z += z;
    this->posVector = DirectX::XMLoadFloat3(&this->pos);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(const DirectX::XMVECTOR& pos)
{
    this->RotVector = pos;
    DirectX::XMStoreFloat3(&this->rotation, pos);
    this->UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
    this->rotation = DirectX::XMFLOAT3(x, y, z);
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);

    this->UpdateViewMatrix();
}



void Camera::AdjustRotation(const DirectX::XMVECTOR& pos)
{
    this->RotVector += pos;
    DirectX::XMStoreFloat3(&this->rotation, pos);
    this->UpdateViewMatrix();
}

void Camera::AdjustRotation(float x, float y, float z)
{
    this->rotation.x += x;
    this->rotation.y += y;
    this->rotation.z += z;
    this->RotVector = DirectX::XMLoadFloat3(&this->rotation);
    UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
    
    // CALCULE LA ROTATION 
    DirectX::XMMATRIX CameraRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.x, this->rotation.y, this->rotation.z);

    //CALCUL LA TARGET  
    DirectX::XMVECTOR CamTarget = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, CameraRotationMatrix);

    CamTarget += this->posVector;

    DirectX::XMVECTOR UpDir = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, CameraRotationMatrix);
    this->ViewMatrix = DirectX::XMMatrixLookAtLH(this->posVector, CamTarget, UpDir);

    DirectX::XMMATRIX vecRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f,this->rotation.y,0.0f);
    this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
    this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
    this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
    this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
    this->vec_down = XMVector3TransformCoord(this->DEFAULT_DOWN_VECTOR, vecRotationMatrix);
    this->vec_up = XMVector3TransformCoord(this->DEFAULT_UP_VECTOR, vecRotationMatrix);

} 




const DirectX::XMMATRIX& Camera::GetViewMatrix() const
{
    return this->ViewMatrix;

}
const DirectX::XMMATRIX& Camera::GetProjectionMatrix() const
{
    return this->ProjectionMatrix;
}   




const DirectX::XMFLOAT3& Camera::GetPos() const
{
    return this->pos;
}

const DirectX::XMVECTOR& Camera::GetVecPos() const
{
    return this->posVector;
}


const DirectX::XMFLOAT3& Camera::GetRotation() const
{
    return this->rotation;
}
