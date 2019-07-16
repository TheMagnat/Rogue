

class Projectile{

	public:

		Projectile(float const& speed, int const& frame, std::string const& fichier, sf::IntRect const& spritePosCote, sf::IntRect const& spritePosVert) : speed_(speed), gravite_(0), frame_(frame), action_(0), spritePosCote_(spritePosCote), spritePosVert_(spritePosVert), fichier_(fichier) {
			if(!texture_.loadFromFile(fichier_)){
				std::cout << "ERREUR" << std::endl;
			}
			else{
				sprite_.setTexture(texture_);
				sprite_.setScale(0.5, 0.5);
			}
		}

		void setOnAction(sf::Vector2f const& pos, int const& x, int const& y){
			sprite_.setPosition(pos);
			sens_.x=x;
			sens_.y=y;
			if(x>0){
				sprite_.setTextureRect(spritePosCote_);
			}
			else if(x<0){
				sprite_.setTextureRect(spritePosCote_);
				sprite_.setScale(-0.5, 0.5);
			}
			else if(y>0){
				sprite_.setTextureRect(spritePosVert_);
				sprite_.setScale(0.5, -0.5);
			}
			else if(y<0){
				sprite_.setTextureRect(spritePosVert_);
			}
			sprite_.setOrigin({ sprite_.getLocalBounds().width/2, sprite_.getLocalBounds().height/2 });
			nextAction();
		}

		bool nextAction(){
			if(++frameCompt_ > frame_){
				gravite_=0;
				frameCompt_ = 0;
				sprite_.setScale(0.5, 0.5);
				return true;
			}

			//0.5*9.85*pow(t,2)
			if(sens_.x!=0){
				sprite_.move(sens_.x*speed_, -1 + 0.5*gravite_*gravite_);
				gravite_+=0.1;
			}
			else if (sens_.y < 0){
				sprite_.move(0, sens_.y*speed_);
				sprite_.setScale(sprite_.getScale().x + 0.025 - (0.35*gravite_*gravite_), sprite_.getScale().y + 0.025 - (0.35*gravite_*gravite_));
				gravite_+=0.02;
			}
			else{
				sprite_.move(0, sens_.y*speed_);
				sprite_.setScale(sprite_.getScale().x + 0.025 - (0.35*gravite_*gravite_), -(-sprite_.getScale().y + 0.025 - (0.35*gravite_*gravite_)));
				gravite_+=0.02;
			}

			
			return false;
		}

		sf::Sprite& getSprite(){
			return sprite_;
		}


	private:
		float speed_;
		float gravite_;

		int frame_;
		int frameCompt_;

		int action_;
		//std::vector<sf::IntRect> spritePos_;
		sf::IntRect spritePosCote_;
		sf::IntRect spritePosVert_;
		sf::Vector2f sens_;

		std::string fichier_;
		sf::Texture texture_;
		sf::Sprite sprite_;
};