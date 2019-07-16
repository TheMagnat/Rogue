
class Ennemie{

	public:
		Ennemie(std::string textu, Animation anim, sf::Vector2f pos, int const& speed = 75, int const& vie = 10)
		: fin_(0), vie_(vie), speed_(speed), enAttaque_(false), danger_(false), frames_(0), vitesseSprite_(0.15), save_(0), anim_(anim) {
			if(!texture_.loadFromFile(textu)){
				std::cerr << "Impossible de charger " << textu << std::endl;
			}
			else{
				sprite_.setTexture(texture_);
				sprite_.setTextureRect(anim_.actuelSprite());
				sprite_.setOrigin({ sprite_.getLocalBounds().width/2, sprite_.getLocalBounds().height/2 });
				sprite_.setPosition(pos);
			}
		}

		void move(sf::Vector2f const& aBouger, float const& elapsedTime){
			if(aBouger.x != 0 || aBouger.y != 0){
				aBouger_ = true;
			}
			if(aBouger.x < 0 && !sens_){
				sens_ = true;
				sprite_.setScale(-1, 1);
			}
			else if(aBouger.x > 0 && sens_){
				sens_ = false;
				sprite_.setScale(1, 1);
			}
			sprite_.move(aBouger.x*speed_*elapsedTime, aBouger.y*speed_*elapsedTime);
		}

		void moveX(float x){
			aBouger_ = true;

			if(x < 0 && !sens_){
				sens_ = true;
				sprite_.setScale(-1, 1);
			}
			else if(x > 0 && sens_){
				sens_ = false;
				sprite_.setScale(1, 1);
			}
			sprite_.move(x*speed_, 0);
		}

		void moveY(float y){
			aBouger_ = true;
			sprite_.move(0, y*speed_);
		}

		void annulMoveX(float x){
			sprite_.move(-x*speed_, 0);
		}

		void annulMoveY(float y){
			sprite_.move(0, -y*speed_);
		}

		void annulMove(sf::Vector2f const& aBouger, float const& elapsedTime){
			sprite_.move(-aBouger.x*speed_*elapsedTime, -aBouger.y*speed_*elapsedTime);
		}

		void attack(){
			enAttaque_ = true;
			enMouvement_ = false;
			anim_.setActuelle(3);
			sprite_.setTextureRect(anim_.actuelSprite());

			temps_.restart();
			save_=0;
			vitesseSprite_=0.05;
		}

		float distance(sf::Vector2f const& point2) const {
			sf::Vector2f point1(sprite_.getPosition());
			float saveX(point2.x - point1.x), saveY(point2.y - point1.y);
			return sqrt(saveX*saveX + saveY*saveY);
		}

		void actualise(){
			
		}

		sf::Sprite& getSprite(){
			int tempo;
			if(vie_ == 0){
				if((tempo = temps_.getElapsedTime().asSeconds()/vitesseSprite_) > save_){
					if(++frames_>=10){
						/*if(frames_>=15){
							fin_=2;
						}*/
						return sprite_;
					}
				}
			}
			else if(enAttaque_){
				if((tempo = temps_.getElapsedTime().asSeconds()/vitesseSprite_) > save_){
					if(++frames_==2){
						SoundHandler::playSwing();
						danger_=true;
					}
					else if(frames_==6){
						danger_=true;
					}
					else if(frames_==7){
						danger_=false;
					}
					else if(frames_==10){
						enAttaque_=false;
						frames_=0;
						anim_.setActuelle(0);
						vitesseSprite_=0.15;

						save_=0;
						temps_.restart();
					}
				}
			}
			else if(aBouger_ != enMouvement_){
				if(aBouger_){
					enMouvement_=true;
					anim_.setActuelle(2);
					sprite_.setTextureRect(anim_.actuelSprite());
					temps_.restart();
					save_=0;
				}
				else{
					enMouvement_=false;
					anim_.setActuelle(0);
					sprite_.setTextureRect(anim_.actuelSprite());
					temps_.restart();
					save_=0;
				}
			}

			if((tempo = temps_.getElapsedTime().asSeconds()/vitesseSprite_) > save_){
				save_ = tempo;
				sprite_.setTextureRect(anim_.nextSprite());
			}

			aBouger_ = false;
			return sprite_;
		}

		void takeHit(){
			if(fin_) return;
			if(!alert_){
				vie_ = 0;
			}
			/*else{
				vie_ -= 2;
			}*/

			if(vie_ <= 0){
				SoundHandler::playDeath();

				fin_ = 1;

				frames_=0;
				vie_ = 0;
				anim_.setActuelle(4);
				sprite_.setTextureRect(anim_.actuelSprite());

				temps_.restart();
				save_=0;
				vitesseSprite_=0.15;
			}
		}

		int getFin() const {
			return fin_;
		}

		sf::FloatRect getHitBox() const {
			sf::Vector2f tempo(sprite_.getPosition());
			return sf::FloatRect(tempo.x - 8, tempo.y + 8, 16, 8);
		}

		sf::FloatRect getHitBoxCorp() const {
			sf::Vector2f tempo(sprite_.getPosition());
			return sf::FloatRect(tempo.x - 8, tempo.y -8, 16, 9);
		}

		sf::Vector2f getPosition() const{
			return sprite_.getPosition();
		}

		bool getDanger() const {
			return danger_;
		}

		bool getEnAttaque() const {
			return enAttaque_;
		}

		bool getSens() const {
			return sens_;
		}

		bool getAlert() const {
			return alert_;
		}

		void setAlert(bool const& alert){
			alert_ = alert;
		}


	private:
		int fin_;

		int vie_;
		int speed_;

		bool alert_; //true si alerté

		bool aBouger_;
		bool enMouvement_; //True si en mouvement
		bool enAttaque_; //True si est en attaque
		bool danger_; //true si la frame est celle de degats
		int frames_; //pour compter les frames

		bool sens_; //true si gauche, false si droite
		sf::Sprite sprite_;
		float vitesseSprite_; //vitesse entre 2 sprite
		sf::Texture texture_;

		int save_;
		sf::Clock temps_;
		Animation anim_; //0 = bouge pas, 1 = crie, 2 = deplacement, 3 = attaque, 4 = mort
};

static std::vector<std::vector<sf::IntRect>> orcRect {{creerVect(0, 0, 32, 32, 32, 10), creerVect(0, 32, 32, 32, 32, 10), creerVect(0, 64, 32, 32, 32, 10), creerVect(0, 96, 32, 32, 32, 10), creerVect(0, 128, 32, 32, 32, 10)}};

class Orc : public Ennemie {

	public:
		Orc(sf::Vector2f pos, int speed = 85, int vie = 10) : Ennemie("ressources/orc.png", Animation(orcRect), pos, speed, vie) {}


	private:

};