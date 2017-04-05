#include "Grid.h"



Grid::Grid()
{
	for (int i = 0; i < 196; i++)
	{
		m_tiles.push_back(Tile(i));
	}
}


Grid::~Grid()
{

}
