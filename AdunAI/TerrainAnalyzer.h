
#ifndef _OAI_TERRAINANALYZER_H_
#define _OAI_TERRAINANALYZER_H_

#include <BWAPI.h>
#include <thread>
#include <vector>
#include <mutex>
#include <atomic>
#include "ResourcesArea.h"
#include "Singleton.hpp"
#include "Bounds.h"

namespace Odarich {

	struct Tile
	{
		bool analyzed;
		bool walkable;
		bool buildable;
		bool explored;

		Tile()
			:
			analyzed(false),
			walkable(false),
			buildable(false)
		{

		}
	};



	typedef std::vector<Tile> TileMap;

	class TerrainAnalyzer : public Singleton<TerrainAnalyzer>
	{
	private:

		int _mapWidth = 0;
		int _mapHeight = 0;
		TileMap _tiles;

		std::thread * _taskAnalyzeMap = nullptr;
		 
		bool _tilesMapReady = false;
		void taskAnalyzeMap();
		void analyzeBaseLocations(std::unique_lock<std::mutex> & locker, ResourcesAreasList & areas);
		bool _taskRunning = false;
		std::atomic<short> _taskStatus;
		void resetKernel(int * kernel);
		short analyzeKernel(int * kernel);
		void setTilesInBoundsAnalyzed(const Bounds & bounds);

	public:

		TerrainAnalyzer();
		void analyzeMap();
		
		void init();
		void dispose();
		void draw(const int & x, const int & y);

		bool isTileMapReady() const;
		const Tile & getTile(const int & x, const int & y);
		TileMap & getTileMap();
		int getMapWidth() const;
		int getMapHeight() const;
		short getTaskStatus() const;
		

	};

}

#endif