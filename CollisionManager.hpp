

class CollisionManager{

	public:
		//CollisionManager(Personnage* joueur, Monde* mondeActuel) : mondeActuel_(mondeActuel), joueur_(joueur) {}


		static int verifCollision(float x, float y){
			int save;
			size_t i, j, taille2, taille;
			sf::FloatRect saveHitBox = joueur_->getHitBox();
			

			save = actualObjets_->collision(saveHitBox);
			if(save != -1){
				if(save==0){
					return 1;
				}
				else if(save==10){
					std::cout << "ON DESCEND\n";
					return 10;
				}
				else if(save==20){
					std::cout << "ON MONTE MDR\n";
					return 20;
				}
			}

			save = actualObjets_->collisionCollectable(saveHitBox);
			if(save != -1){
				if(save == 8){
					std::cout << "PETIT GOLD\n";
				}
				else if(save == 9){
					std::cout << "GROS GOLD\n";
				}
			}

			/*if(colli_->at(saveHitBox.left, saveHitBox.top)) return 1;
			if(colli_->at(saveHitBox.left+saveHitBox.width, saveHitBox.top)) return 1;
			if(colli_->at(saveHitBox.left+saveHitBox.width, saveHitBox.top+saveHitBox.height)) return 1;
			if(colli_->at(saveHitBox.left, saveHitBox.top+saveHitBox.height)) return 1;*/

			taille = actualSalles_->size();
			for(i = 0; i < taille; ++i){
				taille2 = (*actualSalles_)[i].getVecCase().size();
				for(j = 0; j < taille2; ++j){
					if((*actualSalles_)[i].getVecCase()[j].getMur()){

						if(saveHitBox.intersects((*actualSalles_)[i].getVecCase()[j].getHitBox())) return true;
					}
				}
			}

			taille = actualChemins_->size();
			for(i = 0; i < taille; ++i){
				if((*actualChemins_)[i].getMur()){
					if(saveHitBox.intersects((*actualChemins_)[i].getHitBox())) return 1;
				}
			}

			return 0;
		}

		static int generalMur(sf::FloatRect const& hitBox){
			size_t i, j, taille2, taille;	

			taille = actualSalles_->size();
			for(i = 0; i < taille; ++i){
				taille2 = (*actualSalles_)[i].getVecCase().size();
				for(j = 0; j < taille2; ++j){
					if((*actualSalles_)[i].getVecCase()[j].getMur()){

						if(hitBox.intersects((*actualSalles_)[i].getVecCase()[j].getHitBox())) return 1;
					}
				}
			}

			taille = actualChemins_->size();
			for(i = 0; i < taille; ++i){
				if((*actualChemins_)[i].getMur()){
					if(hitBox.intersects((*actualChemins_)[i].getHitBox())) return 1;
				}
			}
			return 0;
		}

		static int generalObjets(sf::FloatRect const& hitBox){
			int save;
			save = actualObjets_->collision(hitBox);
			if(save != -1){
				return 1;
			}
			return 0;
		}

		static bool generalCollision(sf::FloatRect const& hitBox){
			int save;
			size_t i, j, taille2, taille;			

			save = actualObjets_->collision(hitBox);
			if(save != -1){
				return true;
			}

			taille = actualSalles_->size();
			for(i = 0; i < taille; ++i){
				taille2 = (*actualSalles_)[i].getVecCase().size();
				for(j = 0; j < taille2; ++j){
					if((*actualSalles_)[i].getVecCase()[j].getMur()){

						if(hitBox.intersects((*actualSalles_)[i].getVecCase()[j].getHitBox())) return true;
					}
				}
			}

			taille = actualChemins_->size();
			for(i = 0; i < taille; ++i){
				if((*actualChemins_)[i].getMur()){
					if(hitBox.intersects((*actualChemins_)[i].getHitBox())) return true;
				}
			}

			return false;
		}

		static void setMondeActuel(Objets* actualObjets, std::vector<Salle>* actualSalles, std::vector<Case>* actualChemins){
			actualObjets_ = actualObjets;
			actualSalles_ = actualSalles;
			actualChemins_ = actualChemins;
		}

		static void setJoueur(Personnage* joueur){
			joueur_ = joueur;
		}

		static void setColli(TrieCollision* colli){
			colli_ = colli;
		}

	public:
		static Objets* actualObjets_;
		static std::vector<Salle>* actualSalles_;
		static std::vector<Case>* actualChemins_;
		static Personnage* joueur_;

		static TrieCollision* colli_;
};

Objets* CollisionManager::actualObjets_;
std::vector<Salle>* CollisionManager::actualSalles_;
std::vector<Case>* CollisionManager::actualChemins_;
Personnage* CollisionManager::joueur_;

TrieCollision* CollisionManager::colli_;