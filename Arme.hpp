

class Arme{

	public:
		Arme(int dammage) : actif_(false), dammage_(dammage), hitBox_(0, 0, 0, 0) {}

		void setPos(sf::Vector2f const& pos, int const& x){
			hitBox_.top = pos.y;
			hitBox_.left = pos.x;

			if(x>0){
				hitBox_.width = 9;
			}
			else{
				hitBox_.width = -9;
			}
			hitBox_.height = 5;
		}

		void start(){
			actif_=true;
		}

		void stop(){
			actif_=false;
		}

		sf::FloatRect& getHitBox(){
			return hitBox_;
		}


		/*bool getActif(){
			return actif_;
		}*/

		bool isActif(){
			return actif_;
		}


	private:
		bool actif_;
		int dammage_;
		sf::FloatRect hitBox_;
		//hit box 9 * 5

};