
class IA{

	public:
		IA(std::list<std::unique_ptr<Ennemie>>* ennemies, TrieCollision* colli, Personnage* cible) : ennemies_(ennemies), colli_(colli), cible_(cible) {}



		int bestMove(Ennemie* ptrEnnemie, sf::Vector2f zoneCible, float const& elaspedTime){

			int nextTo(1);
			sf::Vector2f pos1(ptrEnnemie->getPosition());

			if(ptrEnnemie->distance(zoneCible) < 100){
				if(pos1.x-1 > zoneCible.x){
					nextTo = 0;
					ptrEnnemie->moveX(-elaspedTime);
					if(CollisionManager::generalCollision(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveX(-elaspedTime);
					}
					if(CollisionManager::generalCollision(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveX(elaspedTime);
					}
				}
				else if (pos1.x+1 < zoneCible.x){
					nextTo = 0;
					ptrEnnemie->moveX(elaspedTime);
					if(CollisionManager::generalMur(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveX(elaspedTime);
					}
					if(CollisionManager::generalObjets(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveX(elaspedTime);
					}
				}

				if(pos1.y-1 > zoneCible.y){
					nextTo = 0;
					ptrEnnemie->moveY(-elaspedTime);
					if(CollisionManager::generalMur(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveY(-elaspedTime);
					}
					if(CollisionManager::generalObjets(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveY(-elaspedTime);
					}
				}
				else if(pos1.y+1 < zoneCible.y){
					nextTo = 0;
					ptrEnnemie->moveY(elaspedTime);
					if(CollisionManager::generalMur(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveY(elaspedTime);
					}
					else if(CollisionManager::generalObjets(ptrEnnemie->getHitBox())){
						ptrEnnemie->annulMoveY(elaspedTime);
					}
				}
			}
			else{
				nextTo = 0;

				//astar(pos1, zoneCible); 

			}

			return nextTo;
		}

		void actualise(float const& elaspedTime){
			int nextTo;
			Ennemie* ptrEnnemie;
			sf::Vector2f savePos(cible_->getPosition());

			for(auto it = ennemies_->begin(), et = ennemies_->end(); it != et; it++){

				ptrEnnemie = it->get();
				if(ptrEnnemie->getFin()){
					if(ptrEnnemie->getFin() == 2){
						ennemies_->erase(it);
						--it;
					}
				}
				else if(ptrEnnemie->getEnAttaque()){
					if(ptrEnnemie->getDanger() && !cible_->getInvu()){
						cible_->takeHit();
					}
					continue;
				}
				else if(!ptrEnnemie->getAlert()){
					if(ptrEnnemie->distance(savePos) > 100){
						continue;
					}
					else{
						if(cible_->getDiscret()){
							if(savePos.x > ptrEnnemie->getPosition().x){
								if(!ptrEnnemie->getSens()){
									ptrEnnemie->setAlert(true);
								}
							}
							else{
								if(ptrEnnemie->getSens()){
									ptrEnnemie->setAlert(true);
								}
							}
						}
						else{
							ptrEnnemie->setAlert(true);
						}
					}
				}
				else{
					if(cible_->getVisible()){
						nextTo = bestMove(ptrEnnemie, cible_->getPosition(), elaspedTime);
						if(nextTo){
							ptrEnnemie->attack();
						}
					}
					else{
						ptrEnnemie->setAlert(false);
					}
				}


			}
		}


	private:
		std::list<std::unique_ptr<Ennemie>>* ennemies_;
		TrieCollision* colli_;
		Personnage* cible_;


};