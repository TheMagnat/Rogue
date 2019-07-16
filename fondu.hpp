

class Fondu {

	public:
		Fondu(int const& parFrame) : transp_(255), actif_(false), sens_(false), rectangle_(sf::Vector2f(400, 300)), parFrame_(parFrame) {
			rectangle_.setFillColor(sf::Color::Black);
			rectangle_.setOrigin({ rectangle_.getLocalBounds().width/2, rectangle_.getLocalBounds().height/2 });
		}

		void setPosition(float const& x, float const& y){
			rectangle_.setPosition(x, y);
		}

		sf::RectangleShape& getRectangle(){
			if(actif_){
				if(sens_){
					transp_ += parFrame_;
					rectangle_.setFillColor(sf::Color(0, 0, 0, transp_));
					if(transp_ >= 255) actif_=false;
				}
				else{
					transp_ -= parFrame_;
					rectangle_.setFillColor(sf::Color(0, 0, 0, transp_));
					if(transp_ <= 0) actif_=false;
				}
			}
			return rectangle_;
		}

		void reset(){
			rectangle_.setFillColor(sf::Color::Black);
			transp_ = 255;
		}

		void setActif(bool actif){
			actif_ = actif;
		}

		bool isActif() const {
			return actif_;
		}

	private:
		int transp_;
		bool actif_;
		bool sens_; //true : devient noir, false : devient transparant

		sf::RectangleShape rectangle_;
		int parFrame_;
};