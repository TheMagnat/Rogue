

class Hud{

	public:
		Hud(std::string fichier, sf::Vector2f vue_) : enMouvement_(0), frame_(0), actif_(false), saveX_(300){
			sf::Vertex tempo;

			if(!texture_.loadFromFile(fichier)){
				std::cerr << "Impossible de charger : " << fichier << std::endl;
			}
			else{
				sprite_.setTexture(texture_);
			}
			
			sprite_.setTextureRect(sf::IntRect(0, 0, 300, 600));
			sprite_.setPosition(vue_.x, 0);

			//Rectangle droit
			/*tempo.position = sf::Vector2f(vue_.x - saveX_, 0);
			tempo.texCoords = sf::Vector2f(200, 0);
			bord_.push_back(tempo);

			tempo.position = sf::Vector2f(vue_.x, 0);
			tempo.texCoords = sf::Vector2f(300, 0);
			bord_.push_back(tempo);

			tempo.position = sf::Vector2f(vue_.x, vue_.y);
			tempo.texCoords = sf::Vector2f(300, 600);
			bord_.push_back(tempo);

			tempo.position = sf::Vector2f(vue_.x - saveX_, vue_.y);
			tempo.texCoords = sf::Vector2f(200, 600);
			bord_.push_back(tempo);*/
		}



		void draw(sf::RenderWindow& app){

			if(enMouvement_==1){
				if((frame_+=5) >= saveX_){
					enMouvement_ = 0;
					frame_ = 0;
				}
				sprite_.move(-5, 0);
			}
			else if(enMouvement_==2){
				if((frame_+=5) >= saveX_){
					enMouvement_ = 0;
					frame_ = 0;
					actif_ = false;
				}
				sprite_.move(5, 0);
			}

			app.draw(sprite_);
			//app.draw(bord_.data(), bord_.size(), sf::PrimitiveType::Quads, &texture_);

		}

		void ouvre(){
			actif_ = true;
			enMouvement_ = 1;
		}

		void ferme(){
			if(enMouvement_ == 0) enMouvement_ = 2;
		}

		bool isActif() const {
			return actif_;
		}


	private:
		//1 si s'ouvre, 2 si se ferme, 0 si ne bouge pas
		int enMouvement_;
		int frame_;
		bool actif_;

		int saveX_;
		sf::Texture texture_;
		sf::Sprite sprite_;
		//std::vector<sf::Vertex> bord_;
};