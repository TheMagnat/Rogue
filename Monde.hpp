

class Monde{

	public:
		Monde(std::string fichier, Personnage* cible, Monde* ptrHaut = nullptr)
		:arriereBuff_(sf::PrimitiveType::Quads, sf::VertexBuffer::Static), premierBuff_(sf::PrimitiveType::Quads, sf::VertexBuffer::Static), iaEnnemies(&ennemies_, &colli_, cible), haut_(ptrHaut), bas_(nullptr) {
			if(!texture_.loadFromFile(fichier)){
				std::cerr << "Impossible de charger : " << fichier << std::endl;
			}
			else{
				objets_.setTexture(&texture_);
			}
			ennemies_.emplace_back(new Orc(sf::Vector2f(432, 332)));
		}

		Salle& operator[](int const& index){
			return salles_[index];
		}

		std::vector<Case>& getChemins(){
			return chemins_;
		}

		std::vector<Salle>& getSalles(){
			return salles_;
		}

		Objets& getObjets(){
			return objets_;
		}

		void push_back(Salle const& aAjouter){
			salles_.push_back(aAjouter);
		}

		size_t size() const{
			return salles_.size();
		}

		bool possibleCheminHaut(int xHaut, int xBas, int haut, int bas){
			int i, j, size1, size2;

			sf::Vector2f pos;
			sf::Vector2f epaisseur;

			pos = salles_[haut].getVecCase()[xHaut - 1].getPos();
			pos.y += 32;

			epaisseur.x = 16 * 4;
			epaisseur.y = salles_[bas].getVecCase()[xBas].getPos().y - pos.y - 32;

			sf::FloatRect hitBox(pos, epaisseur);

			size1=size();

			for(i=0;  i<size1; ++i){
				size2 = salles_[i].getVecCase().size();
				for(j=0; j<size2; ++j){
					if(hitBox.intersects(salles_[i].getVecCase()[j].getHitBox())) return true;
				}
			}
			size1 = chemins_.size();
			for(i=0; i<size1; ++i){
				if(hitBox.intersects(chemins_[i].getHitBox())) return true;
			}
			return false;
		}

		bool possibleCheminCote(int xGauche, int xDroite, int gauche, int droite){
			int i, j, size1, size2;

			sf::Vector2f pos;
			sf::Vector2f epaisseur;

			pos = salles_[gauche].getVecCase()[xGauche - 3*salles_[gauche].getLongueur()].getPos();
			pos.x += 16;

			epaisseur.y = 16 * 6;
			epaisseur.x = salles_[droite].getVecCase()[xDroite].getPos().x - pos.x - 0;

			sf::FloatRect hitBox(pos, epaisseur);

			size1=size();

			for(i=0;  i<size1; ++i){
				size2 = salles_[i].getVecCase().size();
				for(j=0; j<size2; ++j){
					if(hitBox.intersects(salles_[i].getVecCase()[j].getHitBox())) return true;
				}
			}
			size1=chemins_.size();
			for(i=0; i<size1; ++i){
				if(hitBox.intersects(chemins_[i].getHitBox())) return true;
			}
			return false;
		}

		bool collision(int const& longueur, int const& hauteur, sf::Vector2f const& pos){
			sf::IntRect newSalle_(pos.x, pos.y-32, longueur*16, (hauteur+3)*16);

			for(size_t i=0; i < salles_.size(); ++i){
				if(newSalle_.intersects(salles_[i].getBox())) return true;
			}
			return false;
		}

		void remplirAleatoire(int nbSalles){
			int i, j, size, rHaut, rLong, r;
			sf::Vector2f pos(-1, -1);

			rHaut = rand()%5 + 6;
			rLong = rand()%8 + 7;

			salles_.push_back(Salle(&texture_, rLong, rHaut, sf::Vector2f(400, 304)));

			for(i=1; i<nbSalles; ++i){
				genererSalleAleatoire();
			}

			size = salles_.size();

			for(j=0; j<size; ++j){
				for(i=j+1; i<size; ++i){
					cheminEntreHaut(j, i);
					cheminEntreCote(j, i);
				}
			}

			//Toute la map est généré

			//Ajout des escalier pour monter et descendre
			posEscaHaut_ = salles_[0].trouverMurEscalier();
			objets_.push_firstOBjet(sf::IntRect(112, 64, 16, 32), posEscaHaut_, sf::FloatRect(posEscaHaut_.x, posEscaHaut_.y, 16, 32), 20);

			while(pos.x == -1){
				r = rand()%salles_.size();
				pos = salles_[r].trouverMurEscalier();
			}
			posEscaBas_ = pos;
			objets_.push_objet(sf::IntRect(128, 64, 16, 32), pos, sf::FloatRect(pos.x, pos.y, 16, 32), 10);

			decorerLesSalles();

			objets_.remplirMap();

			remplirColli();

			remplirVertexArray();
			arriereBuff_.create(arrierePlan_.size());
			arriereBuff_.update(arrierePlan_.data());

			premierBuff_.create(premierPlan_.size());
			premierBuff_.update(premierPlan_.data());

		}

		void remplirColli(){

			size_t i, j, taille, taille2;
			Case* ptrCase;

			//Parcours des salles
			taille = salles_.size();
			for(i = 0; i<taille; ++i){
				taille2 = salles_[i].getVecCase().size();

				for(j=0; j<taille2; ++j){
					ptrCase = &salles_[i].getVecCase()[j];
					colli_.add(ptrCase->getPos(), ptrCase->getMur());
				}

			}

			taille = chemins_.size();
			for(i=0; i<taille; ++i){
				ptrCase = &chemins_[i];
				colli_.add(ptrCase->getPos(), ptrCase->getMur());
			}

			objets_.remplirColli(colli_);

		}

		void remplirVertexArray(){

			/*arrierePlan_.setPrimitiveType(sf::PrimitiveType::Quads);
			premierPlan_.setPrimitiveType(sf::PrimitiveType::Quads);*/

			size_t i, j, taille(salles_.size()), taille2;
			sf::Vertex tempo;
			sf::IntRect rect;
			sf::Vector2f pos;
			Case* ptrCase;

			for(i=0; i<taille; ++i){
				taille2 = salles_[i].getVecCase().size();
				for(j=0; j<taille2; ++j){

					ptrCase = &salles_[i].getVecCase()[j];
					rect = ptrCase->getTextureRect();
					pos = ptrCase->getPos();
					if(ptrCase->getVisible()){

						//Angle haut droit
						tempo.position = pos;
						tempo.texCoords = sf::Vector2f(rect.left, rect.top);
						premierPlan_.push_back(tempo);

						//Angle haut gauche
						tempo.position = sf::Vector2f(pos.x + 16, pos.y);
						tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
						premierPlan_.push_back(tempo);

						//Angle bas droit
						tempo.position = sf::Vector2f(pos.x + 16, pos.y + 16);
						tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
						premierPlan_.push_back(tempo);

						//Angle bas gauche
						tempo.position = sf::Vector2f(pos.x, pos.y + 16);
						tempo.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
						premierPlan_.push_back(tempo);
					}
					else{
						
						//Angle haut droit
						tempo.position = pos;
						tempo.texCoords = sf::Vector2f(rect.left, rect.top);
						arrierePlan_.push_back(tempo);

						//Angle haut gauche
						tempo.position = sf::Vector2f(pos.x + 16, pos.y);
						tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
						arrierePlan_.push_back(tempo);

						//Angle bas droit
						tempo.position = sf::Vector2f(pos.x + 16, pos.y + 16);
						tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
						arrierePlan_.push_back(tempo);

						//Angle bas gauche
						tempo.position = sf::Vector2f(pos.x, pos.y + 16);
						tempo.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
						arrierePlan_.push_back(tempo);
					}

				}
			}

			taille = chemins_.size();
			for(i=0; i<taille; ++i){

				ptrCase = &chemins_[i];
				rect = ptrCase->getTextureRect();
				pos = ptrCase->getPos();

				if(ptrCase->getVisible()){

					//Angle haut droit
					tempo.position = pos;
					tempo.texCoords = sf::Vector2f(rect.left, rect.top);
					premierPlan_.push_back(tempo);

					//Angle haut gauche
					tempo.position = sf::Vector2f(pos.x + 16, pos.y);
					tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
					premierPlan_.push_back(tempo);

					//Angle bas droit
					tempo.position = sf::Vector2f(pos.x + 16, pos.y + 16);
					tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
					premierPlan_.push_back(tempo);

					//Angle bas gauche
					tempo.position = sf::Vector2f(pos.x, pos.y + 16);
					tempo.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
					premierPlan_.push_back(tempo);
				}
				else{
						
					//Angle haut droit
					tempo.position = pos;
					tempo.texCoords = sf::Vector2f(rect.left, rect.top);
					arrierePlan_.push_back(tempo);

					//Angle haut gauche
					tempo.position = sf::Vector2f(pos.x + 16, pos.y);
					tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top);
					arrierePlan_.push_back(tempo);

					//Angle bas droit
					tempo.position = sf::Vector2f(pos.x + 16, pos.y + 16);
					tempo.texCoords = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
					arrierePlan_.push_back(tempo);

					//Angle bas gauche
					tempo.position = sf::Vector2f(pos.x, pos.y + 16);
					tempo.texCoords = sf::Vector2f(rect.left, rect.top + rect.height);
					arrierePlan_.push_back(tempo);
				}
			}

		}

		void draw(sf::RenderWindow& app, Personnage& joueur_){
			app.draw(arriereBuff_, &texture_);


			objets_.draw(app, joueur_, ennemies_);


			app.draw(premierBuff_, &texture_);
		}

		void decorerLesSalles(){
			int i, taille, r;
			taille = size();
			for(i=0; i<taille; ++i){
				r = rand()%4;
				salles_[i].decorer(r, objets_);
			}

		}

		//Methode pour la generation de salle aleatoire en fonctions de la pos des salles deja créé
		void genererSalleAleatoire(){
			int saveX, saveY, saveLong, saveHaut; //save

			int rHaut, rLong, rIndex, rSens, rTailleChemin, rVariation; //random

			sf::Vector2f pos;
			while(true){
				do{
					rIndex = rand()%salles_.size();
					rSens = rand()%4;
				}
				while(salles_[rIndex].estPris(rSens));

				rHaut = rand()%8 + 6;
				rLong = rand()%11 + 6;
				rTailleChemin = rand()%3;

				saveX = salles_[rIndex].getPos().x;
				saveY = salles_[rIndex].getPos().y;

				if(rSens == 0){ //gauche
					
					if(rHaut > 6 && salles_[rIndex].getHauteur() > 6){
						rVariation = rand()%2;
						if(rand()%2) rVariation *= -1;
					}
					else rVariation = 0;

					pos.x = saveX - rLong*16 - rTailleChemin*16;
					pos.y = saveY + rVariation*16;
				}
				else if(rSens == 1){ //haut

					if(rLong > 6 && salles_[rIndex].getLongueur() > 6){
						rVariation = rand()%2;
						if(rand()%2) rVariation *= -1;
					}
					else rVariation = 0;

					pos.x = saveX + rVariation*16;
					pos.y = saveY - 48 - rHaut*16 - rTailleChemin*16;
				}
				else if(rSens == 2){ //droite

					if(rHaut > 6 && salles_[rIndex].getHauteur() > 6){
						rVariation = rand()%2;
						if(rand()%2) rVariation *= -1;
					}
					else rVariation = 0;

					saveLong = salles_[rIndex].getLongueur();

					pos.x = saveX + saveLong*16 + rTailleChemin*16;
					pos.y = saveY + rVariation*16;
				}
				else if(rSens == 3){ //bas

					if(rLong > 6 && salles_[rIndex].getLongueur() > 6){
						rVariation = rand()%2;
						if(rand()%2) rVariation *= -1;
					}
					else rVariation = 0;

					saveHaut = salles_[rIndex].getHauteur();

					pos.x = saveX + rVariation*16;
					pos.y = saveY + 48 + saveHaut*16 + rTailleChemin*16;
				}

				if(!collision(rLong, rHaut, pos)) break;
			}
			salles_.push_back(Salle(&texture_, rLong, rHaut, pos));
		}

		//Methodes pour relier les salles pouvant l'être
		void cheminEntreHaut(int index1, int index2){
			int i, random, saveIndex;
			sf::IntRect save;
			std::vector<int> choix_;

			if(salles_[index1].getPos().x > salles_[index2].getPos().x){
				saveIndex = index1;
				index1 = index2;
				index2 = saveIndex;
			}

			save = salles_[index1].getBox();
			saveIndex = save.left + save.width;

			if(saveIndex > salles_[index2].getBox().left + salles_[index2].getBox().width){
				save = salles_[index2].getBox();
				saveIndex = save.left + save.width;
			}

			for (i = salles_[index2].getPos().x + 32; i < saveIndex - 48; i+=16){
				choix_.push_back(i);
			}
			if(choix_.size() == 0) return;
			random = rand()%choix_.size();


			if(salles_[index1].getPos().y > salles_[index2].getPos().y){

				if(possibleCheminHaut(salles_[index2].getIndex(choix_[random], salles_[index2].getPos().y + 16 * (salles_[index2].getHauteur()-1)), salles_[index1].getIndex(choix_[random], salles_[index1].getPos().y), index2, index1)) return;

				tracerCheminHaut(salles_[index2].getIndex(choix_[random], salles_[index2].getPos().y + 16 * (salles_[index2].getHauteur()-1)), salles_[index1].getIndex(choix_[random], salles_[index1].getPos().y), index2, index1);
			}

			else{

				if(possibleCheminHaut(salles_[index1].getIndex(choix_[random], salles_[index1].getPos().y + 16 * (salles_[index1].getHauteur()-1)), salles_[index2].getIndex(choix_[random], salles_[index2].getPos().y), index1, index2)) return;

				tracerCheminHaut(salles_[index1].getIndex(choix_[random], salles_[index1].getPos().y + 16 * (salles_[index1].getHauteur()-1)), salles_[index2].getIndex(choix_[random], salles_[index2].getPos().y), index1, index2);
			}
		}

		void cheminEntreCote(int index1, int index2){
			int i, random, saveIndex;
			sf::IntRect save;
			std::vector<int> choix_;

			if(salles_[index1].getPos().y > salles_[index2].getPos().y){
				saveIndex = index1;
				index1 = index2;
				index2 = saveIndex;
			}

			save = salles_[index1].getBox();
			saveIndex = save.top + save.height;

			if(saveIndex > salles_[index2].getBox().top + salles_[index2].getBox().height){
				save = salles_[index2].getBox();
				saveIndex = save.top + save.height;
			}

			for (i = salles_[index2].getPos().y + 32; i < saveIndex - 48; i+=16){
				choix_.push_back(i);
			}
			if(choix_.size() == 0) return;
			random = rand()%choix_.size();


			if(salles_[index1].getPos().x > salles_[index2].getPos().x){

				if(possibleCheminCote(salles_[index2].getIndex(salles_[index2].getPos().x + 16 * (salles_[index2].getLongueur()-1), choix_[random]), salles_[index1].getIndex(salles_[index1].getPos().x, choix_[random]), index2, index1)) return;

				tracerCheminCote(salles_[index2].getIndex(salles_[index2].getPos().x + 16 * (salles_[index2].getLongueur()-1), choix_[random]), salles_[index1].getIndex(salles_[index1].getPos().x, choix_[random]), index2, index1);
			
			}
			else{

				if(possibleCheminCote(salles_[index1].getIndex(salles_[index1].getPos().x + 16 * (salles_[index1].getLongueur()-1), choix_[random]), salles_[index2].getIndex(salles_[index2].getPos().x, choix_[random]), index1, index2)) return;

				tracerCheminCote(salles_[index1].getIndex(salles_[index1].getPos().x + 16 * (salles_[index1].getLongueur()-1), choix_[random]), salles_[index2].getIndex(salles_[index2].getPos().x, choix_[random]), index1, index2);
			
			}
		}

		void tracerCheminHaut(int xGauche, int xDroite, int gauche, int droite){

			salles_[gauche].getVecCase()[xGauche-1 + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(224, 16, 16, 16));	//1 dessus gauche
			salles_[gauche].getVecCase()[xGauche-1].setTextureRect(sf::IntRect(32, 16, 16, 16)); 									//Gauche

			salles_[gauche].getVecCase()[xGauche + salles_[gauche].getLongueur()].setMur(false);
			salles_[gauche].getVecCase()[xGauche + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(80, 112, 16, 16));		//dessus case
			salles_[gauche].getVecCase()[xGauche].setMur(false);
			salles_[gauche].getVecCase()[xGauche].setVisible(false);
			salles_[gauche].getVecCase()[xGauche].setTextureRect(sf::IntRect(336, 128, 16, 16)); 							   	 	//Case


			salles_[gauche].getVecCase()[xGauche+1 + salles_[gauche].getLongueur()].setMur(false);						
			salles_[gauche].getVecCase()[xGauche+1 + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(224, 112, 16, 16));	//dessus droite
			salles_[gauche].getVecCase()[xGauche+1].setMur(false);				
			salles_[gauche].getVecCase()[xGauche+1].setVisible(false);				
			salles_[gauche].getVecCase()[xGauche+1].setTextureRect(sf::IntRect(352, 128, 16, 16));									//droite

			salles_[gauche].getVecCase()[xGauche+2 + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(80, 16, 16, 16));	//2 dessus 2 droite
			salles_[gauche].getVecCase()[xGauche+2].setTextureRect(sf::IntRect(16, 16, 16, 16));									//2 droite

			salles_[gauche].getVecCase()[xGauche - salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(0, 112, 16, 16)); 		//Dessus dessous
			salles_[gauche].getVecCase()[xGauche+1 - salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(0, 112, 16, 16)); 	//Dessus Droite




			salles_[droite].getVecCase()[xDroite-1 - 2 * salles_[droite].getLongueur()].setTextureRect(sf::IntRect(64, 16, 16, 16));//2 dessus gauche
			salles_[droite].getVecCase()[xDroite-1 - salles_[droite].getLongueur()].setTextureRect(sf::IntRect(80, 64, 16, 16));	//dessus gauche
			salles_[droite].getVecCase()[xDroite-1].setTextureRect(sf::IntRect(80, 80, 16, 16)); 									//Gauche

			salles_[droite].getVecCase()[xDroite - 2 * salles_[droite].getLongueur()].setMur(false);
			salles_[droite].getVecCase()[xDroite - 2 * salles_[droite].getLongueur()].setTextureRect(sf::IntRect(80, 112, 16, 16)); //2 dessus case
			salles_[droite].getVecCase()[xDroite - salles_[droite].getLongueur()].setMur(false);
			salles_[droite].getVecCase()[xDroite - salles_[droite].getLongueur()].setTextureRect(sf::IntRect(80, 112, 16, 16));		//dessus case
			salles_[droite].getVecCase()[xDroite].setMur(false);
			salles_[droite].getVecCase()[xDroite].setTextureRect(sf::IntRect(80, 112, 16, 16)); 							   	 	//Case

			salles_[droite].getVecCase()[xDroite+1 - 2*salles_[droite].getLongueur()].setMur(false);						
			salles_[droite].getVecCase()[xDroite+1 - 2*salles_[droite].getLongueur()].setTextureRect(sf::IntRect(224, 112, 16, 16));//2 dessus Droite
			salles_[droite].getVecCase()[xDroite+1 - salles_[droite].getLongueur()].setMur(false);						
			salles_[droite].getVecCase()[xDroite+1 - salles_[droite].getLongueur()].setTextureRect(sf::IntRect(224, 112, 16, 16));	//dessus Droite
			salles_[droite].getVecCase()[xDroite+1].setMur(false);						
			salles_[droite].getVecCase()[xDroite+1].setTextureRect(sf::IntRect(224, 112, 16, 16));									//Droite

			salles_[droite].getVecCase()[xDroite+2 - 2*salles_[droite].getLongueur()].setTextureRect(sf::IntRect(48, 16, 16, 16));	//2 dessus 2 Droite
			salles_[droite].getVecCase()[xDroite+2 - salles_[droite].getLongueur()].setTextureRect(sf::IntRect(0, 64, 16, 16));		//dessus 2 Droite
			salles_[droite].getVecCase()[xDroite+2].setTextureRect(sf::IntRect(0, 80, 16, 16));										//2 Droite

			salles_[droite].getVecCase()[xDroite + salles_[droite].getLongueur()].setTextureRect(sf::IntRect(304, 128, 16, 16)); 	//Case dessous
			salles_[droite].getVecCase()[xDroite+1 + salles_[droite].getLongueur()].setTextureRect(sf::IntRect(320, 128, 16, 16));	//Dessous Droite

			relierCheminHaut(xGauche, xDroite, gauche, droite);
		}

		void tracerCheminCote(int yGauche, int yDroite, int gauche, int droite){

			salles_[gauche].getVecCase()[yGauche - 3*salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(48, 16, 16, 16)); 	//Case 3 dessus
			salles_[gauche].getVecCase()[yGauche - 2*salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(0, 64, 16, 16));  	//Case 2 dessus
			salles_[gauche].getVecCase()[yGauche - salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(0, 80, 16, 16)); 	 	//Case dessus

			salles_[gauche].getVecCase()[yGauche].setMur(false);
			salles_[gauche].getVecCase()[yGauche].setTextureRect(sf::IntRect(128, 112, 16, 16)); 							   	 	//Case

			salles_[gauche].getVecCase()[yGauche + salles_[gauche].getLongueur()].setMurVisible();
			salles_[gauche].getVecCase()[yGauche + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(16, 16, 16, 16)); 	//Case dessous

			salles_[gauche].getVecCase()[yGauche-1].setTextureRect(sf::IntRect(320, 128, 16, 16)); 								//Avant
			salles_[gauche].getVecCase()[yGauche-1 + salles_[gauche].getLongueur()].setTextureRect(sf::IntRect(352, 128, 16, 16)); //Dessous Avant


			salles_[droite].getVecCase()[yDroite - 3*salles_[droite].getLongueur()].setTextureRect(sf::IntRect(64, 16, 16, 16)); 	//Case 3 dessus
			salles_[droite].getVecCase()[yDroite - 2*salles_[droite].getLongueur()].setTextureRect(sf::IntRect(80, 64, 16, 16));  	//Case 2 dessus
			salles_[droite].getVecCase()[yDroite - salles_[droite].getLongueur()].setTextureRect(sf::IntRect(80, 80, 16, 16));		//Case dessus

			salles_[droite].getVecCase()[yDroite].setMur(false);
			salles_[droite].getVecCase()[yDroite].setTextureRect(sf::IntRect(128, 112, 16, 16)); 							   	 	//Case

			salles_[droite].getVecCase()[yDroite + salles_[droite].getLongueur()].setMurVisible();
			salles_[droite].getVecCase()[yDroite + salles_[droite].getLongueur()].setTextureRect(sf::IntRect(32, 16, 16, 16)); 	//Case dessous

			salles_[droite].getVecCase()[yDroite+1].setTextureRect(sf::IntRect(304, 128, 16, 16)); 								//Avant
			salles_[droite].getVecCase()[yDroite+1 + salles_[droite].getLongueur()].setTextureRect(sf::IntRect(336, 128, 16, 16)); //Dessous Avant

			relierCheminCote(yGauche, yDroite, gauche, droite);
		}

		void relierCheminHaut(int quelleY1, int quelleY2, int index1, int index2){
			int i, x, deY, aY, r;

			x = salles_[index2].getVecCase()[quelleY2].getPos().x;
			deY = salles_[index1].getVecCase()[quelleY1].getPos().y+32;
			aY = salles_[index2].getVecCase()[quelleY2].getPos().y-32;

			for(i=deY; i<aY; i+=16){
				r = rand()%3;
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(224 + 16 * r, 16, 16, 16), sf::Vector2f(x-16, i))); 			// gauche

				r = rand()%3;
				chemins_.push_back(Case(false, false, &texture_, sf::IntRect(80 + 16 * r, 112, 16, 16), sf::Vector2f(x, i))); // centre
				r = rand()%3;
				chemins_.push_back(Case(false, false, &texture_, sf::IntRect(224 + 16 * r, 112, 16, 16), sf::Vector2f(x+16, i))); // droite

				r = rand()%3;
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(80 + 16 * r, 16, 16, 16), sf::Vector2f(x+32, i))); // 2 droite
			}
		}

		void relierCheminCote(int quelleY1, int quelleY2, int index1, int index2){
			int i, y, deX, aX, r;

			y = salles_[index2].getVecCase()[quelleY2].getPos().y;
			deX = salles_[index1].getVecCase()[quelleY1].getPos().x+16;
			aX = salles_[index2].getVecCase()[quelleY2].getPos().x;

			for(i=deX; i<aX; i+=16){
				r = rand()%3;
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(176 + 16 * r, 16, 16, 16), sf::Vector2f(i, y-48))); // 3 Haut
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(48 + 16 * (((i-deX)/16)%2), 64, 16, 16), sf::Vector2f(i, y-32))); // 2 Haut
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(48 + 16 * (((i-deX)/16)%2), 80, 16, 16), sf::Vector2f(i, y-16))); // 1 Haut

				r = rand()%3;
				chemins_.push_back(Case(false, false, &texture_, sf::IntRect(128 + 16 * r, 112, 16, 16), sf::Vector2f(i, y))); // Centre

				r = rand()%3;
				chemins_.push_back(Case(false, true, &texture_, sf::IntRect(128 + 16 * r, 16, 16, 16), sf::Vector2f(i, y+16))); // 1 Bas
				chemins_.push_back(Case(true, false, &texture_, sf::IntRect(0, 16, 16, 16), sf::Vector2f(i, y+32))); // 2 Bas
			}
		}
		//fin des methodes de generation de chemins

		void attack(sf::FloatRect const& hitBox){

			for(auto it = ennemies_.begin(), et = ennemies_.end(); it != et; ++it){
				
				if(hitBox.intersects((*it)->getHitBoxCorp())){
					(*it)->takeHit();
				}

			}

		}

		TrieCollision& getColli(){
			return colli_;
		}

		void actualise(float const& elapsedTime){
 			iaEnnemies.actualise(elapsedTime);
		}

		int objetCollision(sf::FloatRect const& hitBox){
			return objets_.collision(hitBox);
		}

		int objetCollisionCollectable(sf::FloatRect const& hitBox){
			return objets_.collisionCollectable(hitBox);
		}

		void drawEnnemies(sf::RenderWindow& app){

			for(auto it = ennemies_.begin(); it != ennemies_.end(); ++it){
				app.draw((*it)->getSprite());
			}
		}

		std::shared_ptr<Monde>& getPtrHaut(){
			return haut_;
		}

		void setPtrHaut(std::shared_ptr<Monde> ptr){
			haut_ = ptr;
		}

		void setPtrHaut(Monde* ptr){
			haut_.reset(ptr);
		}

		sf::Vector2f getPosEscaHaut() const{
			return posEscaHaut_;
		}

		std::shared_ptr<Monde>& getPtrBas(){
			return bas_;
		}

		void setPtrBas(std::shared_ptr<Monde> ptr){
			bas_ = ptr;
		}

		void setPtrBas(Monde* ptr){
			bas_.reset(ptr);
		}

		sf::Vector2f getPosEscaBas() const{
			return posEscaBas_;
		}

	private:
		//Pour l'affichage des murs
		sf::Texture texture_;

		sf::VertexBuffer arriereBuff_;
		sf::VertexBuffer premierBuff_;

		std::vector<sf::Vertex> arrierePlan_;
		std::vector<sf::Vertex> premierPlan_;

		//Pour stocker les salles, chemins, objets
		std::vector<Salle> salles_;
		std::vector<Case> chemins_;
		Objets objets_;

		//Pour un acces rapide et grossier des collisions
		TrieCollision colli_;

		//Pour stocker les ennemies et leur ia
		std::list<std::unique_ptr<Ennemie>> ennemies_;
		IA iaEnnemies;

		//Pointeur vers le prochain monde
		std::shared_ptr<Monde> haut_;
		std::shared_ptr<Monde> bas_;

		//Pour garder en mémoire la position des escaliers
		sf::Vector2f posEscaHaut_;
		sf::Vector2f posEscaBas_;
};