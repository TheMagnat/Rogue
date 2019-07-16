

class Salle{

	public:

		Salle(sf::Texture* texture, int const& longueur, int const& hauteur, sf::Vector2f const& pos, int const& sensPris = -1)
		: longueur_(longueur), hauteur_(hauteur), pos_(pos), box_(pos.x, pos.y-32, longueur*16, (hauteur+2) *16), texture_(texture), sorties_{{false, false, false, false}} {
			initialiseVector(pos_.x, pos_.y);
		}

		Salle(Salle const& aCopier) : longueur_(aCopier.longueur_), hauteur_(aCopier.hauteur_), pos_(aCopier.pos_), box_(aCopier.box_), texture_(aCopier.texture_), sorties_(aCopier.sorties_) {
			initialiseVector(pos_.x, pos_.y);
		}

		void initialiseVector(int const& x, int const& y){
			int i, j, r;

			cases_.push_back(Case(true, false, texture_, sf::IntRect(352, 32, 16, 16), sf::Vector2f(x, y-32)));
			for(i = 1; i < longueur_-1; ++i){
				r = rand()%3;
				cases_.push_back(Case(true, false, texture_, sf::IntRect(176 + 16 * r, 16, 16, 16), sf::Vector2f(x + i * 16, y-32)));
			}
			cases_.push_back(Case(true, false, texture_, sf::IntRect(336, 32, 16, 16), sf::Vector2f(x+i*16, y-32)));

			cases_.push_back(Case(true, false, texture_, sf::IntRect(224, 16, 16, 16), sf::Vector2f(x, y-16)));
			for(i = 1; i < longueur_-1; ++i){
				r = rand()%8;
				if(r==3){
					cases_.push_back(Case(true, false, texture_, sf::IntRect(32, 64, 16, 16), sf::Vector2f(x + i * 16, y-16)));
				}
				else if(r==4){
					cases_.push_back(Case(true, false, texture_, sf::IntRect(16, 64, 16, 16), sf::Vector2f(x + i * 16, y-16)));
				}
				else{
					cases_.push_back(Case(true, false, texture_, sf::IntRect(48 + 16 * (i%2), 64, 16, 16), sf::Vector2f(x + i * 16, y-16)));
				}
				
			}
			cases_.push_back(Case(true, false, texture_, sf::IntRect(80, 16, 16, 16), sf::Vector2f(x+i*16, y-16)));

			cases_.push_back(Case(true, false, texture_, sf::IntRect(224, 16, 16, 16), sf::Vector2f(x, y)));
			for(i = 1; i < longueur_-1; ++i){
				r = rand()%8;
				if(r==3){
					cases_.push_back(Case(true, false, texture_, sf::IntRect(32, 80, 16, 16), sf::Vector2f(x + i * 16, y)));
				}
				else if(r==4){
					cases_.push_back(Case(true, false, texture_, sf::IntRect(16, 80, 16, 16), sf::Vector2f(x + i * 16, y)));
				}
				else{
					cases_.push_back(Case(true, false, texture_, sf::IntRect(48 + 16 * (i%2), 80, 16, 16), sf::Vector2f(x + i * 16, y)));
				}
			}
			cases_.push_back(Case(true, false, texture_, sf::IntRect(80, 16, 16, 16), sf::Vector2f(x+i*16, y)));

			cases_.push_back(Case(true, false, texture_, sf::IntRect(224, 16, 16, 16), sf::Vector2f(x, y+16)));
			cases_.push_back(Case(false, false, texture_, sf::IntRect(16, 112, 16, 16), sf::Vector2f(x+16, y+16)));
			for(i = 2; i < longueur_-2; ++i){
				r = rand()%3;
				cases_.push_back(Case(false, false, texture_, sf::IntRect(128 + 16 * r, 112, 16, 16), sf::Vector2f(x + i * 16, y + 16))); // a changer
			}
			cases_.push_back(Case(false, false, texture_, sf::IntRect(32, 112, 16, 16), sf::Vector2f(x + i * 16, y + 16)));
			cases_.push_back(Case(true, false, texture_, sf::IntRect(80, 16, 16, 16), sf::Vector2f(x + (i+1) * 16, y + 16)));
			
			for(j = 2; j < hauteur_-1; ++j){
				r = rand()%3;
				cases_.push_back(Case(true, false, texture_, sf::IntRect(224 + 16 * r, 16, 16, 16), sf::Vector2f(x, y + j * 16)));
				r = rand()%3;
				cases_.push_back(Case(false, false, texture_, sf::IntRect(80 + 16 * r, 112, 16, 16), sf::Vector2f(x+16, y + j * 16)));
				for(i = 2; i < longueur_-2; ++i){
					r = rand()%6;
					if(r>3){
						r = rand()%8;
					}
					cases_.push_back(Case(false, false, texture_, sf::IntRect(16 + 16 * r, 144, 16, 16), sf::Vector2f(x + i * 16, y + j * 16))); // a changer
					
				}
				r = rand()%3;
				cases_.push_back(Case(false, false, texture_, sf::IntRect(224 + 16 * r, 112, 16, 16), sf::Vector2f(x + i * 16, y + j * 16)));
				r = rand()%3;
				cases_.push_back(Case(true, false, texture_, sf::IntRect(80 + 16 * r, 16, 16, 16), sf::Vector2f(x + (i+1) * 16, y + j * 16)));
				
			}			

			cases_.push_back(Case(true, true, texture_, sf::IntRect(320, 32, 16, 16), sf::Vector2f(x, y + (j) * 16)));
			for(i = 1; i < longueur_-1; ++i){
				r = rand()%3;
				cases_.push_back(Case(false, true, texture_, sf::IntRect(128 + 16 * r, 16, 16, 16), sf::Vector2f(x + i * 16, y + (j) * 16)));
			}
			cases_.push_back(Case(true, true, texture_, sf::IntRect(304, 32, 16, 16), sf::Vector2f(x + i * 16, y + (j) * 16)));

			for(i = 0; i < longueur_; ++i){
				cases_.push_back(Case(true, false, texture_, sf::IntRect(0, 16, 16, 16), sf::Vector2f(x + i * 16, y + (j+1) * 16)));
			}
		}

		sf::Vector2f trouverMurEscalier(){
			int i, r;
			std::vector<int> choix;
			for(i=1+longueur_; i<2*longueur_-1; ++i){
				if(cases_[i].getMur()){
					if(!cases_[i].getPris() && cases_[i - 1].getMur() && cases_[i + 1].getMur()) choix.push_back(i);
				}
			}
			if(choix.empty()){
				return sf::Vector2f(-1, -1);
			}
			r = rand()%choix.size();
			cases_[choix[r]].setPris(true);
			return cases_[choix[r]].getPos();
		}

		sf::Vector2f trouverMur(){
			int i, r;
			std::vector<int> choix;
			for(i=1+longueur_; i<2*longueur_-1; ++i){
				if(cases_[i].getMur()){
					if(!cases_[i].getPris()) choix.push_back(i);
				}
			}
			if(choix.empty()){
				return sf::Vector2f(-1, -1);
			}
			r = rand()%choix.size();
			cases_[choix[r]].setPris(true);
			return cases_[choix[r]].getPos();
		}

		//0 = tous, 1 = seulement le haut, 2 = seulement coté
		sf::Vector2f trouverBord(int const& quelBord = 0){
			int r;
			size_t taille, i;
			std::vector<int> choix;
			taille = 4*longueur_-1;

			if(quelBord != 2){
				for(i=longueur_*3+1; i<taille; ++i){
					if(!cases_[i].getPris() && cases_[i-longueur_].getMur()) choix.push_back(i);
				}
			}
			if(quelBord != 1){
				taille = hauteur_-1;
				for(i=2; i<taille; ++i){
					if(!cases_[1+(i+2)*longueur_].getPris() && cases_[(i+2)*longueur_].getMur()) choix.push_back(1+(i+2)*longueur_);
					if(!cases_[longueur_-2+(i+2)*longueur_].getPris() && cases_[longueur_-1+(i+2)*longueur_].getMur()) choix.push_back(longueur_-2+(i+2)*longueur_);
				}
			}

			if(choix.empty()){
				return sf::Vector2f(-1, -1);
			}
			r = rand()%choix.size();
			cases_[choix[r]].setPris(true);
			return cases_[choix[r]].getPos();
		}

		sf::Vector2f trouverCaseVide(bool const& rendrePris){
			int r;
			size_t i, j, taille1, taille2;
			std::vector<int> choix;

			taille1 = hauteur_-1;
			taille2 = longueur_ -1;
			for(i=1; i<taille1; ++i){
				for(j=1; j<taille2; ++j){
					if(!cases_[j+(i+2)*longueur_].getPris()) choix.push_back(j+(i+2)*longueur_);
				}
			}
			if(choix.empty()) return sf::Vector2f(-1, -1);
			r = rand()%choix.size();
			cases_[choix[r]].setPris(rendrePris);
			return cases_[choix[r]].getPos();
		}

		sf::Vector2f trouverEspaceLibre(int const longueur, int const hauteur) const{
			int r;
			size_t taille, taille2, i, j;
			std::vector<int> choix;

			taille  = hauteur_ - hauteur;
			taille2 = longueur_ - longueur;
			for(i=1; i<taille; ++i){
				for(j=1; j<taille2; ++j){
					if(estLibre(j + (i+2) * longueur_, longueur, hauteur)){
						choix.push_back(j + (i+2) * longueur_);
					}
				}
			}
			if(choix.empty()) return sf::Vector2f(-1, -1);
			r = rand()%choix.size();
			return cases_[choix[r]].getPos();
		}

		//Pour savoir si un groupe de case est libre.
		bool estLibre(int const& index, int const& longueur, int const& hauteur) const{
			int i, j;

			for(i=0; i<hauteur; ++i){
				for(j=0; j<longueur; ++j){
					if(cases_[index + j + i * hauteur_].getPris()) return false;
				}
			}

			return true;
		}

		void decorer(int type, Objets& objets_){
			int i, j, r, r2, save;
			sf::Vector2f tempo, tempo2;

			//Drapeau sur les murs
			if(type != 3){
				r = rand()%2 + 1;
				if(type == 0){
					for(i=0; i<r; ++i){
						objets_.push_objet(sf::IntRect(288, 176, 16, 32), trouverMur());
					}
				}
				else if(type == 1){
					for(i=0; i<r; ++i){
						objets_.push_objet(sf::IntRect(320, 176, 16, 32), trouverMur());
					}
				}
				else if(type == 2){
					for(i=0; i<r; ++i){
						objets_.push_objet(sf::IntRect(352, 176, 16, 32), trouverMur());
					}
				}
			}

			//Torche animé
			r = rand()%3;
			for(i=0; i<r; ++i){
				tempo = trouverMur();
				if(tempo.x != -1){
					objets_.push_animated(sf::IntRect(176, 305, 16, 16), tempo, std::vector<std::vector<sf::IntRect>> {{creerVect(176, 305, 16, 16, 16, 3)}}, 0.2);
				}
			}

			//Decoration au sol
			r = rand()%3;
			if(r == 0){
				r = rand()%3+1;
				for(i=0; i<r; ++i){
					objets_.push_deco(sf::IntRect(96, 240, 16, 16), trouverCaseVide(false));
				}
			}
			r = rand()%3;
			if(r == 0){
				r = rand()%3+1;
				for(i=0; i<r; ++i){
					objets_.push_deco(sf::IntRect(112, 240, 16, 16), trouverCaseVide(false));
				}
			}
			r = rand()%3;
			if(r == 0){
				r = rand()%3+1;
				for(i=0; i<r; ++i){
					objets_.push_deco(sf::IntRect(128, 240, 16, 16), trouverCaseVide(false));
				}
			}
			//fin decoration au sol

			//Decoration spéciale pour salle assez grande
			if(hauteur_>10 && longueur_>10){
				std::cout << "TRES GRAND\n";
				tempo = trouverEspaceLibre(9, 9);


				save=getIndex(tempo.x+16, tempo.y+16);
				cases_[save].setPris(true);
				cases_[save].setTextureRect(sf::IntRect(352, 128, 16, 16));
				for(i=tempo.x+32; i<tempo.x+80; i+=16){
					r=rand()%3;
					save=getIndex(i, tempo.y+16);
					cases_[save].setPris(true);
					cases_[save].setTextureRect(sf::IntRect(176 + 16 * r, 112, 16, 16));
				}
				save=getIndex(i, tempo.y+16);
				cases_[save].setPris(true);
				cases_[save].setTextureRect(sf::IntRect(336, 128, 16, 16));

				for(i=tempo.y+32; i<tempo.y+80; i+=16){
					r=rand()%3;
					save=getIndex(tempo.x+16, i);
					cases_[save].setPris(true);
					cases_[save].setTextureRect(sf::IntRect(224 + 16 * r, 112, 16, 16));
					for(j=tempo.x+32; j<tempo.x+80; j+=16){
						save=getIndex(j, i);
						cases_[save].setPris(true);
						cases_[save].setTextureRect(sf::IntRect(144 + 16 * (int)((i-tempo.y)/64), 144, 16, 16));
					}
					r=rand()%3;
					save=getIndex(j, i);
					cases_[save].setPris(true);
					cases_[save].setTextureRect(sf::IntRect(80 + 16 * r, 112, 16, 16));
				}

				save=getIndex(tempo.x+16, i);
				cases_[save].setPris(true);
				cases_[save].setTextureRect(sf::IntRect(320, 128, 16, 16));
				for(j=tempo.x+32; j<tempo.x+80; j+=16){
					r=rand()%3;
					save=getIndex(j, i);
					cases_[save].setPris(true);
					cases_[save].setTextureRect(sf::IntRect(128 + 16 * r, 112, 16, 16));
				}
				save=getIndex(j, i);
				cases_[save].setPris(true);
				cases_[save].setTextureRect(sf::IntRect(304, 128, 16, 16));

				objets_.push_objet(sf::IntRect(336, 304, 16, 32), sf::Vector2f(tempo.x+48, tempo.y+28), sf::FloatRect(tempo.x+48, tempo.y+43, 16, 19));
			}

			if(hauteur_>7 && longueur_>7){
				r = rand()%5;
				if(r < 5){
					tempo = trouverEspaceLibre(6, 5);
					if(tempo.x != -1){
						//Table
						cases_[getIndex(tempo.x+32, tempo.y+16)].setPris(true);
						objets_.push_objet(sf::IntRect(288, 247, 32, 25), sf::Vector2f(tempo.x+32, tempo.y+16), sf::FloatRect(tempo.x+32, tempo.y+16 + 7, 32, 14));
						//Chaise 1
						r = rand()%2;
						cases_[getIndex(tempo.x+64 - 48 * r, tempo.y+16)].setPris(true);
						objets_.push_objet(sf::IntRect(212 + 15 * r, 250, 9, 22), sf::Vector2f(tempo.x+64 - 48 * r, tempo.y+16), sf::FloatRect(tempo.x+64 - 48 * r, tempo.y+16 + 11, 9, 7));
						//Chaise 2
						r = rand()%2;
						cases_[getIndex(tempo.x+32 + 16 * r, tempo.y+32)].setPris(true);
						objets_.push_objet(sf::IntRect(243, 250, 10, 18), sf::Vector2f(tempo.x+32 + 16 * r, tempo.y+32), sf::FloatRect(tempo.x+32 + 16 * r, tempo.y+32 + 7, 10, 7));
					}
				}
			}

			//Tonneau/sac/caisse
			r = rand()%2;
			if(r == 1) r = rand()%5;
			for(i=0; i<r; ++i){
				r2 = rand()%3;
				if(r2==0){
					tempo = trouverBord();
					if(tempo.x != -1) objets_.push_objet(sf::IntRect(2, 305, 12, 15), tempo, sf::FloatRect(tempo.x, tempo.y+8, 12, 7));
				}
				else if(r2==1){
					tempo = trouverBord();
					if(tempo.x != -1) objets_.push_objet(sf::IntRect(67, 305, 10, 15), tempo, sf::FloatRect(tempo.x, tempo.y+8, 10, 7));
				}
				else{
					r2 = rand()%3;
					tempo = trouverBord();
					if(tempo.x != -1) objets_.push_objet(sf::IntRect(96 + 16 * r2, 304, 16, 16), tempo);
				}
			}

			//LES golds
			while(rand()%7 == 0){
				r = rand()%2;
				tempo = trouverEspaceLibre(1, 1);
				cases_[getIndex(tempo.x, tempo.y)].setPris(true);
				objets_.push_collectable(sf::IntRect(0 + 16 * r, 352, 16, 16), tempo, sf::FloatRect(tempo.x, tempo.y, 16, 16), 8 + r);
			}

		}

		int getIndex(int const& x, int const& y){
			int i;
			for(i=0; i<longueur_; ++i){
				if(cases_[i].getPos().x == x){
					for(; i < longueur_ * (hauteur_+2); i += longueur_){
						if(cases_[i].getPos().y == y){
							return i;
						}
					}
				}
			}
			return -1;
		}

		bool estPris(int const& index){
			return sorties_[index];
		}

		std::vector<Case>& getVecCase(){
			return cases_;
		}

		sf::Vector2f& getPos(){
			return pos_;
		}

		int& getLongueur(){
			return longueur_;
		}

		int& getHauteur(){
			return hauteur_;
		}

		sf::IntRect& getBox(){
			return box_;
		}


	private:
		int longueur_;
		int hauteur_;
		sf::Vector2f pos_;
		sf::IntRect box_;

		std::string fichier_;
		sf::Texture* texture_;

		std::vector<Case> cases_;

		//Info next
		std::array<bool, 4> sorties_; // gauche, haut, droite, bas
};