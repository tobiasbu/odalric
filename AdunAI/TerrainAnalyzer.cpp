
#include "TerrainAnalyzer.h"
#include "TaskCommons.h"
#include "ResourcesAreaManager.h"
#include "BaseManager.h"
#include "Log.h"
#include "Utils.h"
#include "Draw.h"
#include "Math.h"

#define MAX_RESOURCE_INFLUENCE_RADIUS 10

namespace Odalric {
	
	TerrainAnalyzer::TerrainAnalyzer()
	{
		_taskStatus = 0; // "None";
		_tilesMapReady = false;
	}

	void TerrainAnalyzer::analyzeMap()
	{

	}

	void TerrainAnalyzer::resetKernel(int * kernel)
	{
		for (int k = 0; k < 12; k++)
			kernel[k] = -1;
	}

	short TerrainAnalyzer::analyzeKernel(int * kernel)
	{
		//bool b22 = false;
		//bool b32 = false;
		bool b43 = true;
		int index = 0;
		int x = 0, y = 0;

		//Log::info("kernel");


		for (; y < 3; y++)
		{
			//index = x + y * 4;

			//Log::info("%d %d %d %d", kernel[index]);

			for (x = 0; x < 4; x++)
			{
				index = x * 3 + y;//x + y * 4;

				if (kernel[index] != 0)
				{
					b43 = false;
					break;

				}
			}
		}

		if (!b43)
			return 0;
		else
			return 4;
	}

	void TerrainAnalyzer::setTilesInBoundsAnalyzed(const Bounds & bounds)
	{
		int index = 0;
		for (int y = bounds[0].y; y < bounds[1].y; y++)
		{
			for (int x = bounds[0].x; x < bounds[1].x; x++)
			{
			
				index = x  * _mapHeight + y;

				_tiles[index].analyzed = true;
			}
		}

	}

	void TerrainAnalyzer::taskAnalyzeMap()
	{
		while (_taskRunning)
		{

			if (!_tilesMapReady)
			{
				_taskStatus = 1; // "Getting TileMap";

				int index = 0;
				int j = 0;
				int i = 0;
				bool walk = false;

				// get all tiles to set the tile map
				for (int y = 0; y < _mapHeight; y++)
				{
					for (int x = 0; x < _mapWidth; x++)
					{


						index = x  *_mapHeight + y; //+ y * _mapWidth;

						_tiles[index].analyzed = false;
						_tiles[index].buildable = BWAPI::Broodwar->isBuildable(x, y, true);

						walk = false;

						// walkable tiles is 8x8 so 32 / 8 = 4
						for (i = 0; i < 4; i++)
						{
							for (j = 0; j < 4; j++)
							{
								walk = BWAPI::Broodwar->isWalkable(x * 4 + i, y * 4 + j);

								if (!walk)
									break;
							}
						}


						_tiles[index].walkable = walk;

					}

				}

				// set not buildable for tiles where has minerals and geysers
				auto minerals = BWAPI::Broodwar->getMinerals();
				auto gas = BWAPI::Broodwar->getGeysers();
				Bounds b;

				for each (auto var in minerals)
				{
					b.setByType(var->getType(),var->getTilePosition());

					for (int y = b[0].y; y < b[1].y; y++)
					{
						for (int x = b[0].x; x < b[1].x; x++)
						{
							index = x  *_mapHeight + y;
							_tiles[index].buildable = false;
							//_tiles[index].walkable =
						}

					}
					
				}

				for each (auto var in gas)
				{
					b.setByType(var->getType(), var->getTilePosition());

					for (int y = b[0].y; y < b[1].y; y++)
					{
						for (int x = b[0].x; x < b[1].x; x++)
						{
							index = x  *_mapHeight + y;
							_tiles[index].buildable = false;
							_tiles[index].walkable = false;
						}

					}

				}

				_taskStatus = 2; //store("TileMap completed");

				_tilesMapReady = true;

			}
			else if (_taskStatus == 2)
			{
				_taskStatus = 3;
				

				if (ResourcesAreaManager::getInstance().getTaskStatus() == 2 && _taskStatus == 3)
				{
					std::unique_lock<std::mutex> lock(TaskCommons::mutex);
					ResourcesAreasList & areas = ResourcesAreaManager::getInstance()._resourcesAreas;
					
					lock.unlock();
					analyzeBaseLocations(lock,areas);
					_taskStatus = 4;
				}

				_taskRunning = false;

				//const ResourcesAreas & areas = ResourcesAreaManager::getInstance().getResourcesAreas();
				//BasesMap & map = BaseManager::getInstance().getBases();
				//BasesMap::iterator it = map.begin();
				
				
				

				/*while (it != map.end())
				{
					if (!it->first)
					{
						it++;
						continue;
					}

					if (!it->second.hasBuildBounds())
					{
						ResourcesArea * r = it->second.getResources();

						if (r != nullptr)
						{
							
							
							Bounds areaBounds = r->getBounds();

							// convert to tile position



							areaBounds[0].x /= 32;
							areaBounds[0].y /= 32;
							areaBounds[1].x /= 32;
							areaBounds[1].y /= 32;

							Bounds resourceBounds = areaBounds;

							areaBounds.grow(40);

							areaBounds.limit(0,0,_mapWidth,_mapHeight);
							int kindex = 0;
							int tindex = 0;
							int tx = 0;
							int ty = 0;

							Log::info("b size: %d %d %d %d", areaBounds[0].x, areaBounds[0].y, areaBounds[1].x, areaBounds[1].y);

							
							//BWAPI::UnitFilter filter(BWAPI::UnitTypes::Resource_Mineral_Field,);

							// analize tiles 2x2, 3x2, 4x3
							for (int y = areaBounds[0].y; y < areaBounds[1].y; y++)
							{
								for (int x = areaBounds[0].x; x < areaBounds[1].x; x++)
								{

								resetKernel(kernel);

								for (int ky = 0; ky < 3; ky++)
								{
								for (int kx = 0; kx < 4; kx++)
								{
										
												
											int tx = kx + x;
											int ty = ky + y;

											//if (tx < 0 || ty < 0 || tx > _mapWidth || ty > _mapHeight)
											//{
											//	continue;
											//}
											//else
											//{
											kindex = kx + ky * 4;

											if (resourceBounds.intersects(tx, ty, tx, ty))
											{
												kernel[kindex] = 1;
												continue;
											}
											else {

													

													tindex =  tx * _mapHeight + ty; // tx + ty * _mapWidth;
													 
													Tile & tile = _tiles[tindex];

													if (!tile.analyzed)
													{

														if (tile.buildable)
														{
															// BWAPI::UnitTypes::Resource_Mineral_Field // BWAPI::Filter::IsMineralField
															//auto set = BWAPI::Broodwar->getUnitsOnTile(tx, ty);
															

															//if (set.size() == 0)
															//	kernel[kindex] = 0;
															
																kernel[kindex] = 0;

																///*for each (const auto & var in set)
																//{
																//	if (var->getType().isMineralField() || var->getType().isBuilding()) {
																//		kernel[kindex] = 1;
																//		break;
																//	}
																//	 
																//		
																//}
																
															
														}
														else 
														{
															kernel[kindex] = 1;
														}

														

													}
													else
														continue;
											}

													 
											//}
										}
									}

								short result = analyzeKernel(kernel);

								if (result == 4)
								{
									//Log::info("BOUNDS ADDED");

									Bounds b;
									b.set(x, y, x + 4, y + 3);
									setTilesInBoundsAnalyzed(b);
									b[0].x *= 32;
									b[0].y *= 32;
									b[1].x *= 32;
									b[1].y *= 32;

									it->second.addBuildBounds(b);
									x += 3;
								}

								
								}
							}

							it->second.setHasBuildBounds(true);
						}
						else
							it++;

						

						}
						else
							it++;

					}*/

				_taskStatus = 5;
				}

			
			}

		

		
	}

	void TerrainAnalyzer::analyzeBaseLocations(std::unique_lock<std::mutex> & locker, ResourcesAreasList & areas)
	{
		//Log::info("%d", areas.size());

		ResourcesAreasList::iterator it = areas.begin();
		int kernel[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		int kindex = 0;
		int tindex = 0;
		// for each resources area, we will found base location
		while (it != areas.end())
		{

			// dont have base
			if ((*it)->getBase() == nullptr)
			{
				const BWAPI::Unitset & minerals = (*it)->getMinerals();
				const BWAPI::Unitset & geysers = (*it)->getGas();
				//BWAPI::Unitset::iterator itMine = minerals.begin();
				//const BWAPI::Region & areaRegion = BWAPI::Broodwar->getRegionAt((*itMine)->getPosition());
				//areaRegion->
				//BWAPI::Region areaRegion = (*itMine)->getRegion();

				/*if (!areaRegion)
				{
					Log::error("region dont exist");
				}*/

				std::vector<Bounds> areaBuildableBase;
				Bounds areaBounds = (*it)->getBounds();
				areaBounds[0] /= 32;
				areaBounds[1] /= 32;

				//Log::info("%d", bounds[0].x);

				int xgrow = 2, ygrow = 2;

				if (areaBounds.getSize().x < 12)
					xgrow = 12 - areaBounds.getSize().x;

				if (areaBounds.getSize().y < 12)
					ygrow = 12 - areaBounds.getSize().y;

				areaBounds.grow(xgrow, ygrow);
				areaBounds.limit(0, 0, _mapWidth - 1, _mapHeight - 1);


				//B bounds
				for (int y = areaBounds[0].y; y < areaBounds[1].y; y++)
				{
					for (int x = areaBounds[0].x; x < areaBounds[1].x; x++)
					{
						resetKernel(kernel);

						// 4x3
						for (int ky = 0; ky < 3; ky++)
						{
							for (int kx = 0; kx < 4; kx++)
							{
								int tx = kx + x;
								int ty = ky + y;

								if (tx < 0 || ty < 0 || tx >= _mapWidth || ty >= _mapHeight)
									continue;

								

								// is the same region
								
									tindex = tx * _mapHeight + ty;

									const Tile & tile = _tiles[tindex];

									// kernel index
									kindex = kx * 3 + ky;// kx + ky * 4;

									if (tile.buildable)
									{
										//if (areaRegion->getID() == BWAPI::Broodwar->getRegionAt(tx*4+32, ty*4+32)->getID())
											kernel[kindex] = 0;
									}
									else
									{
										kernel[kindex] = 1;
									}
								
							}
						}

						// anaylze kernel
						if (analyzeKernel(kernel) == 4)
						{
							Bounds b;
							b.set(x, y, x + 4, y + 3);
							areaBuildableBase.push_back(b);
						}

					}
				}

				
				//int maxSquareDistance =9 * 9* 2;

				if (!areaBuildableBase.empty())
				{
					const int MAX_INFLUENCE_SQUARED = (MAX_RESOURCE_INFLUENCE_RADIUS+3)*32; // (MAX_RESOURCE_INFLUENCE_RADIUS + 3);// *2 * 32;
					std::vector<Bounds>::iterator itBounds;
					std::unordered_map<Bounds*, int> boundsScore;
					//std::vector<Bounds> resourcesBounds;
					int distx = 0;
					int disty = 0;

					for each (const auto & gas in geysers)
					{
						Bounds gasBounds(gas->getType(),gas->getTilePosition());
						
						//Bounds growedineralBounds = mineralBounds;
						//growedineralBounds.grow(MAX_RESOURCE_INFLUENCE_RADIUS);

						//Log::info("%d, %d", mineralBounds[0].x, mineralBounds[0].x * 32);

						itBounds = areaBuildableBase.begin();

						while (itBounds != areaBuildableBase.end())
						{

							Math::manhattan(gasBounds, *itBounds, distx, disty);

							if (distx < 3 && disty < 3) // 3 * 32 tile
							{
								std::unordered_map<Bounds*, int>::iterator itscore = boundsScore.find(&*itBounds);
								if (itscore != boundsScore.end())
								{
									//if (itscore->first || itscore->first != nullptr)
									//	delete itscore->first;

									boundsScore.erase(itscore);
								}

								itBounds = areaBuildableBase.erase(itBounds);

								
							}
							else {

								int sqrDist = Math::pointDistance(itBounds->centroid().x*32, itBounds->centroid().y*32, gasBounds*32);

								Bounds * theBounds = &(*itBounds);

								if (boundsScore.find(theBounds) == boundsScore.end())
								{
									boundsScore[theBounds] = MAX_INFLUENCE_SQUARED - sqrDist;//Math::max(MAX_INFLUENCE_SQUARED - sqrDist, 0) * 3;//(13 - sqrDist);//(maxSquareDistance - sqrDist) * 3;
								}
								else 
								{
									boundsScore[theBounds] += MAX_INFLUENCE_SQUARED - sqrDist;//Math::max(MAX_INFLUENCE_SQUARED - sqrDist, 0) * 3; //(13 - sqrDist);//(maxSquareDistance - sqrDist) * 3;
								}

								itBounds++;
							}

							
						}

					}

					// now for each mineral and buildable bounds, will remove closest bounds
					for each (const auto& mineral in minerals)
					{

						//BWAPI::TilePosition pos = mineral->getTilePosition();
						Bounds mineralBounds(mineral->getType(), mineral->getTilePosition());

						//Bounds growedineralBounds = mineralBounds;
						//growedineralBounds.grow(MAX_RESOURCE_INFLUENCE_RADIUS);

						//Log::info("%d, %d", mineralBounds[0].x, mineralBounds[0].x * 32);

						itBounds = areaBuildableBase.begin();

						while (itBounds != areaBuildableBase.end())
						{
							//if (growedineralBounds.intersects(*itBounds)) {
							Math::manhattan(mineralBounds, *itBounds, distx, disty);

				
							if (distx < 3 && disty < 3)
							{
								std::unordered_map<Bounds*, int>::iterator itscore = boundsScore.find(&*itBounds);
								if (itscore != boundsScore.end())
								{
									//if (itscore->first || itscore->first != nullptr)
									//	delete itscore->first;
									
									boundsScore.erase(itscore);
								}

									itBounds = areaBuildableBase.erase(itBounds);

									
							}
							else
							{

								int sqrDist = Math::pointDistance(itBounds->centroid().x*32, itBounds->centroid().y*32, mineralBounds*32);

								Bounds * theBounds = &(*itBounds);

								if (boundsScore.find(theBounds) == boundsScore.end())
								{
									boundsScore[theBounds] = MAX_INFLUENCE_SQUARED - sqrDist;// Math::max(MAX_INFLUENCE_SQUARED - sqrDist, 0); // 13 - sqrDist;
									//boundsScore[theBounds].second = disty;
								}
								else
								{
									boundsScore[theBounds] += MAX_INFLUENCE_SQUARED - sqrDist;//Math::max(MAX_INFLUENCE_SQUARED - sqrDist, 0);// (13 - sqrDist); ;
									//boundsScore[theBounds].second += disty;
								}
								itBounds++;

							}

						}
					}

					// find the best tile position for this resource
					int boundsSize = areaBuildableBase.size();

					if (boundsSize > 0)
					{
						int maxScore =  std::numeric_limits<int>::lowest();
						std::unordered_map<Bounds*, int>::iterator itScore = boundsScore.begin();
						std::vector<Bounds*> candidates;
						

						// get the max score
						while (itScore != boundsScore.end())
						{
							if ((*itScore).second > maxScore)
							{
								maxScore = (*itScore).second;
								
								//theFinalBounds = (*itScore).first;
							}
							itScore++;
						}

						// add candidates
						itScore = boundsScore.begin();

						while (itScore != boundsScore.end())
						{
							if (itScore->second >= maxScore || itScore->first->intersects((*it)->getBounds() / 32))
							{
								candidates.push_back(itScore->first);
							}

							itScore++;
						}

						// get the candidate that is more near to the center of this resource area
						int closeDist = std::numeric_limits<int>::max();
						const Bounds * theFinalBounds = nullptr;

						for each (auto candidate in candidates)
						{
							int dist = Math::roundedDistance(candidate->centroid() * 32, (*it)->getBounds().centroid());

							if (dist < closeDist)
							{
								theFinalBounds = candidate;
								closeDist = dist;
							}
						}

						if (theFinalBounds != nullptr)
						{
							locker.lock();
							(*it)->_depotPosition = BWAPI::TilePosition((*theFinalBounds)[0].x, (*theFinalBounds)[0].y);
							locker.unlock();
						}

						//for (int i = 0; i < candidates.size(); i++)
						//	BaseManager::getInstance().getMainBase()->addBuildBounds(*candidates[i]);
						
						// clear data
						candidates.clear();
						areaBuildableBase.clear();
						itScore = boundsScore.begin();

						while (!boundsScore.empty())
						{ 
							/*if (itScore->first || itScore->first != nullptr)
							{
								delete itScore->first;
								
							} */

							itScore = boundsScore.erase(itScore);
							
						
						}

						

						
						



						/*BWAPI::UnitType mineralType = BWAPI::UnitTypes::Resource_Mineral_Field;
						BWAPI::UnitType gasType = BWAPI::UnitTypes::Resource_Vespene_Geyser;

						// for each minerals, we create min bounds
						for each (const auto & mineral in minerals)
						{
						Bounds b;
						BWAPI::TilePosition tile = mineral->getTilePosition();
						b[0].x = tile.x - 3;
						b[0].y = tile.y - 3;
						b[1].x = tile.x + mineralType.tileWidth() + 3;
						b[1].y = tile.y + mineralType.tileHeight() + 3;
						resourcesBounds.push_back(b);
						}

						for each (const auto & gas in geysers)
						{
						Bounds b;
						BWAPI::TilePosition tile = gas->getTilePosition();
						b[0].x = tile.x - 3;
						b[0].y = tile.y - 3;
						b[1].x = tile.x + gasType.tileWidth() + 3;
						b[1].y = tile.y + gasType.tileHeight() + 3;
						resourcesBounds.push_back(b);
						}

						for each (const auto & resourceBounds in resourcesBounds)
						{
						itBounds = areaBuildableBase.begin();

						while (itBounds != areaBuildableBase.end())
						{
						//if (growedineralBounds.intersects(*itBounds)) {
						if (resourceBounds.intersects(*itBounds))
						{
						itBounds = areaBuildableBase.erase(itBounds);
						} else
						itBounds++;



						}
						}*/



						//for (int i = 0; i < candidates.size(); i++)
						//	BaseManager::getInstance().getMainBase()->addBuildBounds(*candidates[i]);

						//for (int i = 0; i < boundsSize; i++)
						//	BaseManager::getInstance().getMainBase()->addBuildBounds(areaBuildableBase[i]);
							


					}
					
				}

				

				
			}


			it++;
		}

		BaseManager::getInstance().getMainBase()->setHasBuildBounds(true);
	}

	void TerrainAnalyzer::init()
	{

		_mapWidth = BWAPI::Broodwar->mapWidth();
		_mapHeight = BWAPI::Broodwar->mapHeight();

		_tiles.reserve(_mapWidth * _mapHeight);

		std::fill(_tiles.begin(), _tiles.end(), Tile());
		_taskRunning = true;
		_taskAnalyzeMap = new std::thread(&TerrainAnalyzer::taskAnalyzeMap, this);
		
	}

	void TerrainAnalyzer::dispose()
	{
		if (_taskAnalyzeMap != nullptr)
		{
			_taskRunning = false;

			if (_taskAnalyzeMap->joinable()) 
				_taskAnalyzeMap->join();

			delete _taskAnalyzeMap;

			

			_taskAnalyzeMap = nullptr;
		}
	}

	void TerrainAnalyzer::draw(const int & x, const int & y)
	{
		std::stringstream ss;
		//if (!_taskStatus.is_lock_free())
		ss << "Terrain Analyzer: ";
		std::string s;

		if (_taskStatus == 1)
			s = "Getting TileMap";
		else if (_taskStatus == 2)
			s = "TileMap completed";
		else if (_taskStatus == 3)
			s = "Waiting";
		else if (_taskStatus == 5)
			s = "Finished Bounds";
		ss << s;
		BWAPI::Broodwar->drawTextScreen(x, y, ss.str().c_str());
	
		/*iBWAPI::Position pos = BWAPI::Broodwar->getScreenPosition();

		nt maxCamW = 640;//320 * 2;
		int maxCamH = 360;//180 * 2;

		if (_tilesMapReady)
		{
			for (int xx = 0; xx < maxCamW; xx += 32)
			{
			for (int yy = 0; yy < maxCamH; yy += 32)
			{
				
					int xtile = (pos.x + xx) / 32;
					int ytile = (pos.y + yy) / 32;

					if (!getTile(xtile, ytile).buildable)
					{

						//xtile *= 32;
						//ytile *= 32;

						//BWAPI::Broodwar->drawBoxMap(xtile, ytile, xtile + 32, ytile + 32, BWAPI::Colors::Red, true);
						Draw::tile(xtile, ytile, BWAPI::Colors::Red, true);

					}
					else
						continue;
				}
			}

		}*/

	}

	bool TerrainAnalyzer::isTileMapReady() const
	{
		return _tilesMapReady;
	}

	TileMap & TerrainAnalyzer::getTileMap()
	{
		return _tiles;
	}

	const Tile & TerrainAnalyzer::getTile(const int & x, const int & y)
	{
		// x + y * _mapWidth
		return _tiles[x  *_mapHeight + y];
	}

	int TerrainAnalyzer::getMapWidth() const
	{
		return _mapWidth;
	}

	int TerrainAnalyzer::getMapHeight() const
	{
		return _mapHeight;
	}

	short TerrainAnalyzer::getTaskStatus() const
	{
		return _taskStatus;
	}

}