#include <fstream>
#include "Map.h"
#include "Graphics.h"

#include <iostream>

using namespace std;

Map::Map(Graphics * graphics)
{
	_graphics = graphics;
	_startx = 0;
	_starty = 0;

	_tile.resize(MapHeight);
	for (int i = 0; i < MapHeight; i++){
		_tile[i].resize(MapWidth);
	}

	UnLoadMap();
}

Map::~Map()
{
	UnLoadMap();
}

// Map
// -1 : Black area
// 
int Map::LoadMap(string filename)
{
	ifstream mapFile(filename.c_str());

	if (!mapFile.good())
		return 1;

	_filename = filename;

	mapFile >> _currentlevelname;
	mapFile >> _nextfilename;

	mapFile >> _startx;
	mapFile >> _starty;

	int tmp = 0;

	//Validation
	int BoxCount = 0;
	int cBoxCount = 0;

	for (int i = 0; i< MapWidth; ++i) {
		for (int j = 0; j< MapHeight; ++j) {
			if (!mapFile.eof()) {
				mapFile >> _tile[i][j];

				if (_tile[i][j] == Target)
					cBoxCount++;

				mapFile >> tmp;
				
				if (tmp) {
					Box tb;
					tb.x = i;
					tb.y = j;
					_box.push_back(tb);
					BoxCount++;
				}

			}
			else {
				return 1;
			}
		}
	}

	for (int i = 0; i < MapWidth; i++){
		for (int j = 0; j < MapHeight; j++){
			cout << _tile[i][j] << ' ';
		}
		cout << '\n';
	}
	mapFile.close();


	if (BoxCount != cBoxCount){
		return 2;
	}
		
	return 0;
}

void Map::UnLoadMap(void)
{
	for (int j = 0; j < MapHeight; j++){
		for (int i = 0; i < MapWidth; i++){
			_tile[j][i] = -1;
		}
	}

	_box.clear();
}

void Map::Draw(void)
{
	
	for (int j = 0; j < MapHeight; j++){
		for (int i = 0; i < MapWidth; i++){
			if (_tile[j][i] > -1){
				_graphics->Draw(TILE, _tile[j][i], j * OBJ_WIDTH, i * OBJ_HEIGHT);
			}
		}
	}

	for (vector<Box>::iterator itr = _box.begin(); itr != _box.end(); itr++){
		if (_tile[itr->x][itr->y] == Target){
			_graphics->Draw(OBJ, BoxComplete, itr->x * OBJ_WIDTH, itr->y * OBJ_HEIGHT);
		}
		else{
			_graphics->Draw(OBJ, BoxUnComplete, itr->x * OBJ_WIDTH, itr->y * OBJ_HEIGHT);
		}
	}

	return;
}

int Map::CheckWin(void)
{
	for (vector<Box>::iterator itr = _box.begin(); itr != _box.end(); itr++){
		if (_tile[itr->x][itr->y] != Target){
			return false;
		}
	}
	return true;
}

int Map::GetTile(int x, int y)
{
	if (x < 0 || x > MapWidth){
		return -1;
	}
	if (y < 0 || y > MapHeight){
		return -1;
	}
	return _tile[x][y];
}

int Map::FindBox(int x, int y)
{
	if (_box.size() < 0){
		return -1;
	}

	for (size_t i = 0; i < _box.size(); i++){
		if (_box[i].x == x && _box[i].y == y){
			return i;
		}
	}
	return -1;
}

int Map::GetStartX()const
{
	return _startx;
}

int Map::GetStartY()const
{
	return _starty;
}

string Map::GetNextFileName()const
{
	return _nextfilename;
}
