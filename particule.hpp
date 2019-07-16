

class Particule {

	public:
		Particule(sf::Texture const& texture, std::vector<sf::IntRect> const& rectText, sf::Vector2f pos, float const& tempsEntreSprite = 0.10)
		: tempsEntreSprite_(tempsEntreSprite), actuel_(0), rectText_(rectText), sprite_(texture, rectText_[0]) {

			sprite_.setOrigin({ sprite_.getLocalBounds().width/2, sprite_.getLocalBounds().height/2 });
			sprite_.setPosition(pos);
			sprite_.setScale(2, 2);

		}

		//true si on atteind la fin du vector de IntRect
		bool actualise(){
			if(temps_.getElapsedTime().asSeconds() > tempsEntreSprite_){

				if(++actuel_ == rectText_.size()) return true;
				sprite_.setTextureRect(rectText_[actuel_]);
				temps_.restart();
			}
			return false;
		}

		sf::Sprite& getSprite(){
			return sprite_;
		}


	private:
		float tempsEntreSprite_;
		size_t actuel_;
		sf::Clock temps_;
		std::vector<sf::IntRect> rectText_;
		sf::Sprite sprite_;
};