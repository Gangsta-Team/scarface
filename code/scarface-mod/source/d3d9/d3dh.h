#ifndef _D3DH_H
#define _D3DH_H

#include <d3d9.h>
#include <DirectXMath.h>
#include <radkey.hpp>

/*static math::Vector ProjectWorldToScreen(const math::Vector& position)
{
  auto currentProxy = Direct3DDevice9Proxy::GetCurrentProxy();
  D3DVIEWPORT9 vp;
  D3DMATRIX mView, mProj;
  currentProxy->GetViewport(&vp);
  currentProxy->GetTransform(D3DTS_VIEW, &mView);
  currentProxy->GetTransform(D3DTS_PROJECTION, &mProj);

  auto posn_xm3 = DirectX::XMFLOAT3(&position.x);
  auto posn = DirectX::XMLoadFloat3(&posn_xm3);

  auto shit1 = DirectX::XMFLOAT4X4(&mView.m[0][0]);
  auto shit2 = DirectX::XMFLOAT4X4(&mProj.m[0][0]);

  auto xmView = DirectX::XMLoadFloat4x4(&shit1);
  auto xmProj = DirectX::XMLoadFloat4x4(&shit2);

  auto result = DirectX::XMVector3Project(posn, vp.X, vp.Y, vp.Width, vp.Height, 0.f, 1.f, xmProj, xmView, DirectX::XMMatrixIdentity());

  return { result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]};
}*/

#endif // !_D3DX_INCLUDE_H