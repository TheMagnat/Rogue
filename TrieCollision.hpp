
class TrieCollision{

	public:

		/*float distance(sf::Vector2f depart, sf::Vector2f arrive){
			float saveX(arrive.x - depart.x), saveY(arrive.y - depart.y);
			return sqrt(saveX*saveX + saveY*saveY);
		}

		sf::Vector2f astar(sf::Vector2f depart, sf::Vector2f arrive){


			
		}*/

		/*void algoAstar(sf::Vector2f actuel, sf::Vector2f arrive, uint8_t mouvements){

		}*/

		void add(sf::Vector2f newPos, bool colli){

			++newPos.x;
			++newPos.y;

			if(newPos.x < 0){
				newPos.x -= 16;
			}
			if(newPos.y < 0){
				newPos.y -= 16; 
			}

			newPos.x /= 16;
			newPos.y /= 16;

			cordToIndex_[newPos.x][newPos.y] = colli;
		}

		bool operator[](sf::Vector2f pos){
			int x(pos.x), y(pos.y);

			if(x<0){
				x -= 16;
			}
			if(y<0){
				y -= 16;
			}

			x /= 16;
			y /= 16;

			return cordToIndex_[pos.x/16][pos.y/16];
		}

		bool at(int x, int y){

			if(x<0){
				x -= 16;
			}
			if(y<0){
				y -= 16;
			}

			x /= 16;
			y /= 16;

			return cordToIndex_[x][y];
		}

	private:

		std::unordered_map<int, std::unordered_map<int, bool>> cordToIndex_;
};