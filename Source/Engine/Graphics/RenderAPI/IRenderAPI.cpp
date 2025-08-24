#include "Engine/Graphics/RenderAPI/IRenderAPI.h"
#include "Engine/Graphics/RenderAPI/DX11/DX11RenderAPI.h"
#include "Engine/Common/Vertex.h"
#include <stdexcept>

using namespace Atlas;

std::unique_ptr<IRenderAPI> IRenderAPI::Create(BackEnd _api)
{
	using enum BackEnd;

	switch (_api)
	{
	case DirectX11:
		return std::make_unique<DX11RenderAPI>();
		break;
	default:
		throw std::runtime_error("対応していないグラフィックAPIが選択されました。");
		break;
	}
}