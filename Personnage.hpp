
static std::vector<sf::IntRect> smoke_rect(creerVect(0, 0, 16, 16, 16, 8));

class Personnage{

	public:

		Personnage(std::string textu, Animation anim, sf::Vector2f pos, int vie) :
		vie_(vie), arme_(nullptr), invu_(false), action_(false), frames_(0), inverse_(false), sens_(0), frameAnnim_(0.15), anim_(anim), save_(0), enMarche_(false) {
			if(!texture_.loadFromFile(textu)){
				std::cerr << "Impossible de charger " << textu << std::endl;
			}
			else{
				sprite_.setTexture(texture_);
				sprite_.setTextureRect(anim.actuelSprite());
				sprite_.setOrigin({ sprite_.getLocalBounds().width/2, sprite_.getLocalBounds().height/2 });
				sprite_.setScale(1,1);
			}
			sprite_.setPosition(pos);

			for(auto i=4; i<8; ++i){
				pas_.push_back(sf::SoundBuffer());
				if(!pas_.back().loadFromFile("ressources/pas/footstep0"+std::to_string(i)+".ogg")){
					std::cerr << "Erreur chargement " << "ressources/pas/footstep0"+std::to_string(i)+".ogg" << std::endl;
				}
			}

			textureSmoke_.loadFromFile("ressources/smoke1.png");
		}

		void takeHit(){
			SoundHandler::playHurt();
			invu_ = true;
			invuAffi_ = false;
			comptInvu_=0;
			vie_ -= 1;
		}

		bool getInvu() const {
			return invu_;
		}

		void getArme(Arme* arme){
			arme_ = arme;
		}

		bool move(float& x, float& y, bool const& sneak){

			if(action_) return false;

			bool change(false);
			int tempo(0);

			if(sneak != sneak_){
				if(sneak){
					sneak_=true;
					frameAnnim_=0.30;
					sonPas_.setVolume(50);
				}
				else{
					sneak_=false;
					frameAnnim_=0.15;
					sonPas_.setVolume(100);
				}
				save_ = -1;
				save_pas_ = -1;
				tempsPas_.restart();
				temps_.restart();
			}

			if(x > 0) tempo = 3;
			else if(x < 0) tempo = -3;

			if(y > 0) tempo = 1;
			else if(y < 0) tempo = 2;

			if(tempo != sens_){
				change = true;

				if(tempo == -3){
					inverse_=true;
					sprite_.setScale(-1, 1);
					anim_.setActuelle(3);
				}
				else{
					if(inverse_ && tempo != 0){
						inverse_=false;
						sprite_.setScale(1, 1);
					}
					anim_.setActuelle(tempo);

				}

				//Annimation
				temps_.restart();
				save_=0;
				updateMarche();
				sens_=tempo;

				//S'occupe des sons
				if(enMarche_){
					if(x==0 && y==0){
						enMarche_=false;
						sonPas_.stop();
					}
				}
				else{
					if(x!=0 || y!=0){
						enMarche_=true;
						tempsPas_.restart();
						save_pas_ = 0;
						sonPas_.setBuffer(pas_[rand()%3]);
						sonPas_.play();
						discret_=false;
					}
				}

			}
			sprite_.move(x, y);
			return change;
		}

		void annulMov(float const& x, float const& y){
			sprite_.move(-x, -y);
		}

		void lanceSmoke(std::list<Particule*>& particules){
			if(!invisible_ && !action_){
				SoundHandler::playSmoke();
				sprite_.setColor(sf::Color(0, 0, 0, 100));
				particules.push_back(new Particule(textureSmoke_, smoke_rect, sprite_.getPosition(), 0.15));
				invisible_ = true;
				temps_invi_.restart();
			}
		}

		void attack(int const& x){
			if(arme_ && !action_){
				//Tout ce qu'il faut arreter
				if(invisible_){
					invisible_ = false;
					sprite_.setColor(sf::Color(255, 255, 255, 255));
				}
				if(enMarche_){
					enMarche_=false;
					sonPas_.stop();
				}
				//Suite

				action_=true;
				sens_=0;
				sf::Vector2f tempo(sprite_.getPosition());

				if(x > 0){
					tempo.x += 6;
					if(inverse_){
						inverse_=false;
						sprite_.setScale(1, 1);
					}
				}

				if(x < 0){
					tempo.x -= 6;
					if(!inverse_){
						inverse_=true;
						sprite_.setScale(-1, 1);
					}
				}
				arme_->setPos(tempo, x);
				anim_.setActuelle(4);
				frameAnnim_ = 0.08;
			}
		}


		void updateMarche(){
			sprite_.setTextureRect(anim_.actuelSprite());
		}

		void nextMarche(){
			if(action_){
				if(++frames_ == 2){
					SoundHandler::playSwing();
				}
				else if(frames_==6){
					arme_->start();
				}
				else if(frames_==7){
					arme_->stop();
				}
				else if(frames_==10){
					action_=false;
					frames_=0;
					anim_.setActuelle(0);
					if(sneak_)frameAnnim_ = 0.3;
					else frameAnnim_ = 0.15;
				}
			}
			sprite_.setTextureRect(anim_.nextSprite());
		}

		void restart(){
			temps_.restart();
			save_=0;
		}

		sf::Sprite& drawSprite(){
			discret_=true;

			int tempo(temps_.getElapsedTime().asSeconds()/frameAnnim_);

			//Si invulnerable
			if(invu_){
				if(++comptInvu_ > 60){
					invu_ = false;
					if(!invisible_) sprite_.setColor(sf::Color(255, 255, 255, 255));
					else sprite_.setColor(sf::Color(0, 0, 0, 100));
				}
				else if(((int)(comptInvu_/7))%2 == 0){
					if(!invisible_) sprite_.setColor(sf::Color(255, 255, 255, 255));
					else sprite_.setColor(sf::Color(0, 0, 0, 100));
				}
				else{
					sprite_.setColor(sf::Color(255, 255, 255, 0));
				}
			}

			//Si l'invisibilité s'arrete
			if(invisible_ && temps_invi_.getElapsedTime().asSeconds() > 3){
				invisible_=false;
				sprite_.setColor(sf::Color(255, 255, 255, 255));
			}

			//S'occupe de l'annimation
			if(tempo > save_){
				save_ = tempo;
				nextMarche();
			}

			//S'occupe du son des pas
			if(enMarche_){
				if((tempo = tempsPas_.getElapsedTime().asSeconds()/(frameAnnim_*4)) > save_pas_){
					save_pas_ = tempo;
					sonPas_.stop();
					sonPas_.setBuffer(pas_[rand()%pas_.size()]);
					sonPas_.play();
					discret_=false;
				}
			}

			return sprite_;
		}

		sf::Sprite& getSprite(){
			return sprite_;
		}

		sf::FloatRect getHitBox(){
			sf::Vector2f tempo(sprite_.getPosition());
			tempo.x -= 7;
			tempo.y += 8;

			sf::FloatRect hitbox(tempo, sf::Vector2f(14, 8));
			return hitbox;
		}

		sf::Vector2f getPosition() const{
			return sprite_.getPosition();
		}

		void setPosition(sf::Vector2f const pos){
			sprite_.setPosition(pos);
		}

		void setPosition(float const x, float const y){
			sprite_.setPosition(x, y);
		}

		bool getVisible() const{
			return !invisible_;
		}

		bool getDiscret() const{
			if(sneak_) return true;
			else{
				return discret_;
			}
		}

		Arme* getArme(){
			return arme_;
		}

	private:
		int vie_;
		Arme* arme_;

		bool sneak_;
		bool discret_;
		bool invisible_;
		sf::Clock temps_invi_;

		bool invu_; //Si est invulnerable après un coup par exemple
		bool invuAffi_; //Pour le clogniotement d'invulnerabilité
		int comptInvu_;

		bool action_;
		int frames_;

		bool inverse_;
		int sens_; // 0 Rien,  1 Bas, 2 Haut, 3 Cote, -3 cote gauche, 4 action
		sf::Texture texture_;
		sf::Sprite sprite_;

		float frameAnnim_;
		Animation anim_;
		sf::Clock temps_;
		int save_;

		//Son pas
		std::vector<sf::SoundBuffer> pas_;
		sf::Sound sonPas_;
		sf::Clock tempsPas_;
		int save_pas_;
		bool enMarche_;

		//test
		sf::Texture textureSmoke_;
};