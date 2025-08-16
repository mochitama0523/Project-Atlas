#pragma once
#include <variant>
#include <vector>

namespace Atlas {

enum class VertexType
{
	Pos,
};

struct VertexPos
{

};

using Vertices = std::variant<std::vector<VertexPos>>;

} //namespace Atlas