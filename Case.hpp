
class Case{

	public:
		Case(bool const mur, bool const visible, sf::Texture* const& texture, sf::IntRect const& posTextu, sf::Vector2f const& pos)
		: mur_(mur), visible_(visible), pris_(false), pos_(pos), TextureRect_(posTextu), hitBox_(pos, sf::Vector2f(16, 16)) {}

		void setTextureRect(sf::IntRect const& posTextu){
			TextureRect_ = posTextu;
		}

		sf::IntRect getTextureRect(){
			return TextureRect_;
		}

		sf::FloatRect getHitBox(){
			return hitBox_;
		}

		sf::Vector2f getPos() const {
			return pos_;
		}

		void setMur(bool choix){
			mur_=choix;
		}

		bool getMur() const{
			return mur_;
		}

		void setVisible(bool const& visible){
			visible_ = visible;
		}

		bool getVisible(){
			return visible_;
		}

		void setMurVisible(){
			mur_ = false;
			visible_ = true;
		}

		bool getPris() const{
			return pris_;
		}

		void setPris(bool const& pris){
			pris_ = pris;
		}

	private:
		bool mur_;
		bool visible_;

		bool pris_;

		sf::Vector2f pos_;
		sf::IntRect TextureRect_;
		sf::FloatRect hitBox_;
};