
class SoundHandler {

	public:
		static void initialiserSons(std::vector<std::string> fichiers){
			size_t i, taille;
			taille = fichiers.size();
			for(i=0; i<taille; ++i){
				sons_.emplace_back();
				if(!sons_.back().loadFromFile(fichiers[i])){
					std::cerr << "Impossible de charger : " << fichiers[i] << std::endl;
				}
			}

			lecteur_.emplace_back();
		}

		static void playCoin(){
			findLecteur(rand()%2);
		}

		static void playSmoke(){
			findLecteur(rand()%5+2);
		}

		static void playSwing(){
			findLecteur(rand()%3+7);
		}

		static void playDeath(){
			findLecteur(rand()%9+10);
		}

		static void playHurt(){
			findLecteur(19);
		}

	private:
		static void findLecteur(int indexAjouer){
			size_t save(0), i, taille;
			if(lecteur_[0].getStatus() != sf::SoundSource::Status::Stopped){
				taille = lecteur_.size();
				for(i=0; i<taille; ++i){
					if(lecteur_[i].getStatus() == sf::SoundSource::Status::Stopped){
						lecteur_[i].setBuffer(sons_[indexAjouer]);
						lecteur_[i].play();
					}
				}
				if(!save){
					lecteur_.emplace_back();
					lecteur_[taille].setBuffer(sons_[indexAjouer]);
					lecteur_[taille].play();
				}
			}
			else{
				lecteur_[0].setBuffer(sons_[indexAjouer]);
				lecteur_[0].play();
			}
		}



	public:
		//0 - 1 gold, 2 - 6 smoke, 7 - 9 swing,
		static std::vector<sf::SoundBuffer> sons_;
		static std::vector<sf::Sound> lecteur_;

};

std::vector<sf::SoundBuffer> SoundHandler::sons_;
std::vector<sf::Sound> SoundHandler::lecteur_;