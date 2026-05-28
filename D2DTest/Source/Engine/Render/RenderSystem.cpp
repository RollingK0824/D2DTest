#include "Engine/Render/RenderSystem.h"
#include "Engine/Graphic/GraphicManager.h"
#include <algorithm>

bool RenderSystem::Initialize()
{
	m_commands.reserve(1000);
	return true;
}

void RenderSystem::SubmitCommand(const RenderCommand& command)
{
	m_commands.push_back(command);
}

void RenderSystem::Release()
{
	m_commands.clear();
	// 임시 vector를 만들어 swap 함수 종료 시 완전 삭제
	std::vector<RenderCommand>().swap(m_commands);
}

void RenderSystem::Render()
{
	if (m_commands.empty())return;

	// zOrde 기준으로 오름차순 정렬
	std::sort(m_commands.begin(), m_commands.end(),
		[](const RenderCommand& a, const RenderCommand& b) {
			return a.zOrder < b.zOrder;
		});

	ID2D1HwndRenderTarget* pRT = GraphicManager::GetInstance()->GetRenderTarget();
	if (!pRT) return;

	// 기본 행렬 백업 연산 전 기본 상태 저장
	D2D1_MATRIX_3X2_F originMatrix;
	pRT->GetTransform(&originMatrix);

	for (const auto& cmd : m_commands)
	{
		if (!cmd.pTexture) continue;

		D2D1_SIZE_F imgSize = cmd.pTexture->GetSize();

		// 좌우 상하 반전 플래그에 따라 크기 배율에 마이너스 곱
		float scaleX = cmd.flipX ? -cmd.scaleX : cmd.scaleX;
		float scaleY = cmd.flipY ? -cmd.scaleY : cmd.scaleY;

		// 이미지 회전 및 반전의 중심이 될 자체 로컬 중심점 계산
		D2D1_POINT_2F localCenter = D2D1::Point2F(imgSize.width / 2.0f, imgSize.height / 2.0f);

		// SRT (크기 배율 -> 회전 -> 월드 이동) 행렬 조합 연산
		D2D1_MATRIX_3X2_F transformMatrix =
			D2D1::Matrix3x2F::Scale(scaleX, scaleY, localCenter) *
			D2D1::Matrix3x2F::Rotation(cmd.rotation, localCenter) *
			D2D1::Matrix3x2F::Translation(cmd.position.x - localCenter.x, cmd.position.y - localCenter.y);

		// 최종 행렬 주입
		pRT->SetTransform(transformMatrix);

		// 소스 영역 전체 지정 구조 설정
		D2D1_RECT_F srcRect = D2D1::RectF(0.0f, 0.0f, imgSize.width, imgSize.height);
		D2D1_RECT_F destRect = D2D1::RectF(0.0f, 0.0f, imgSize.width, imgSize.height);

		// 최종 렌더 타겟에 스프라이트 드로우 명령 하달
		pRT->DrawBitmap(
			cmd.pTexture,
			destRect,
			cmd.opacity, // 알파 브렌딩 투명도 수치 적용
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, // 픽셀 보간 모드 설정
			srcRect
		);
	}

	// RenderTarget 원상 복구 
	pRT->SetTransform(originMatrix);

	m_commands.clear();
}
