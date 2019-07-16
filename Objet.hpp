
class Objets{

	public:
		Objets() = default;

		Objets(sf::Texture* texture) : texture_(texture) {}

		void setTexture(sf::Texture* const& adressTextu){
			texture_=adressTextu;
		}

		int findIndice(int y) const {
			size_t i, taille;
			for(i=0, taille = y_.size(); i<taille; ++i){
				if(y < y_[i]) return i;
			}
			return i;
		}

		void push_firstOBjet(sf::IntRect posTextu, sf::Vector2f pos, sf::FloatRect hitBox = sf::FloatRect(0, 0, 0, 0), int type = 0){
			type_.emplace_back(type);
			y_.emplace_back(pos.y);

			sprites_.emplace_back(*texture_, posTextu);
			sprites_.back().setPosition(pos);

			pos_.emplace_back(pos);

			hitBoxs_.emplace_back(hitBox);
		}

		void push_objet(sf::IntRect posTextu, sf::Vector2f pos, sf::FloatRect hitBox = sf::FloatRect(0, 0, 0, 0), int type = 0){
			int y = pos.y;
			int indice;
			if(y > y_.back()){
				type_.emplace_back(type);
				y_.emplace_back(y);

				sprites_.emplace_back(*texture_, posTextu);
				sprites_.back().setPosition(pos);

				pos_.emplace_back(pos);

				hitBoxs_.emplace_back(hitBox);
			}
			else{
				indice = findIndice(y);

				type_.emplace(type_.cbegin()+indice, type);
				y_.emplace(y_.cbegin()+indice, y);

				sprites_.emplace(sprites_.cbegin()+indice, *texture_, posTextu);
				sprites_[indice].setPosition(pos);

				pos_.emplace(pos_.cbegin()+indice, pos);

				hitBoxs_.emplace(hitBoxs_.cbegin()+indice, hitBox);
			}
		}

		void push_deco(sf::IntRect const& posTextu, sf::Vector2f pos){
			spritesDeco_.emplace_back(*texture_, posTextu);
			spritesDeco_.back().setPosition(pos);
		}

		void push_collectable(sf::IntRect const& posTextu, sf::Vector2f pos, sf::FloatRect const& hitBox, int type){
			spriteColle_.emplace_back(*texture_, posTextu);
			spriteColle_.back().setPosition(pos);

			hitBoxColle_.emplace_back(hitBox);

			typeColle_.emplace_back(type);
		}

		void push_animated(sf::IntRect const& posTextu, sf::Vector2f pos, std::vector<std::vector<sf::IntRect>> const& anim, float cbFrame){
			spriteAnimated_.emplace_back(*texture_, posTextu);
			spriteAnimated_.back().setPosition(pos);

			animation_.emplace_back(anim);
		}

		void remplirMap(){
			size_t i, taille;

			int save(y_[0]);
			yToIndex_[save/16] = 0;

			for(i=1, taille=y_.size(); i<taille; ++i){
				if(y_[i] > save){
					save = y_[i];
					yToIndex_[save/16] = i;
				}
			}
		}

		void remplirColli(TrieCollision& colli){
			size_t i, taille;
			bool quatre;
			sf::FloatRect hitBoxTempo;
			sf::Vector2f posTempo;

			taille = y_.size();

			for(i=0; i<taille; ++i){
				hitBoxTempo = hitBoxs_[i];
				if(hitBoxTempo.width != 0){

					posTempo.x = hitBoxTempo.left;
					posTempo.y = hitBoxTempo.top;

					colli.add(posTempo, true); 

					quatre = false;
					if(hitBoxTempo.width > 16){
						colli.add(sf::Vector2f(posTempo.x+16, posTempo.y), true); 
						quatre = true;
					}

					if(hitBoxTempo.height > 13){
						colli.add(sf::Vector2f(posTempo.x, posTempo.y+16), true); 
						if(quatre){
							colli.add(sf::Vector2f(posTempo.x+16, posTempo.y+16), true);
						}
					}

				}


			}

		}

		int collisionCollectable(sf::FloatRect const& hitBox){
			int save;
			size_t i, taille;

			taille = hitBoxColle_.size();

			for(i=0; i<taille; ++i){
				if(hitBox.intersects(hitBoxColle_[i])){
					save = typeColle_[i];

					spriteColle_.erase(spriteColle_.begin()+i);
					hitBoxColle_.erase(hitBoxColle_.begin()+i);
					typeColle_.erase(typeColle_.begin()+i);

					SoundHandler::playCoin();

					return save;
				}
			}
			return -1;
		}


		int collision(sf::FloatRect const& hitBox){
			size_t i, taille;
			int tempo(y_[0]/16);

			int yMin = (hitBox.top-32)/16;
			int yMax = hitBox.top+64;
			i=yToIndex_[yMin];
			while(i==0){
				--yMin;
				i=yToIndex_[yMin];
				if(yMin < tempo) break;
			}
			taille=sprites_.size();
			for(tempo = y_[i]; i<taille && tempo <= yMax; ++i, tempo = y_[i]){
				if(hitBoxs_[i].intersects(hitBox)){
					return type_[i];
				}
			}
			return -1;
		}


		void draw(sf::RenderWindow& app, Personnage& Joueur, std::list<std::unique_ptr<Ennemie>>& ennemies){ //8 autour du millieu
			size_t i, taille;
			bool anim(false), bJoueur(true), bEnnemies(true);
			int y, yJoueur(Joueur.getPosition().y), yEnnemie, tempo(y_[0]/16);
			std::list<std::unique_ptr<Ennemie>>::iterator it, et;

			if(!ennemies.empty()){
				it = ennemies.begin();
				et = ennemies.end();
				yEnnemie = (*it)->getPosition().y;
			}
			else bEnnemies = false;

			taille = spritesDeco_.size();
			for(i=0; i<taille; ++i){
				app.draw(spritesDeco_[i]);
			}

			taille = spriteColle_.size();
			for(i=0; i<taille; ++i){
				app.draw(spriteColle_[i]);
			}

			taille = spriteAnimated_.size();
			if(temps_.getElapsedTime().asSeconds() > 0.2){
				anim = true;
				temps_.restart();
			}
			for(i=0; i<taille; ++i){
				if(anim){
					spriteAnimated_[i].setTextureRect(animation_[i].nextSprite());
				}
				app.draw(spriteAnimated_[i]);
			}

			//On recherche l'indexe le plus proche de 8 case au dessus du centre
			y = (yJoueur-128)/16;
			i=yToIndex_[y];
			while(i==0){
				--y;
				i=yToIndex_[y];
				if(y < tempo) break;
			}
			
			taille=sprites_.size();
			for(tempo = y_[i]; i<taille && tempo < yJoueur+128; ++i, tempo = y_[i]){
				if(bJoueur && yJoueur < tempo){
					while(bEnnemies && yEnnemie < yJoueur){
						app.draw((*it)->getSprite());
						++it;
						if(it == et) bEnnemies = false;
					}
					app.draw(Joueur.drawSprite());
					bJoueur = false;
				}
				while(bEnnemies && yEnnemie < tempo){
					app.draw((*it)->getSprite());
					++it;
					if(it == et) bEnnemies = false;
				}
				app.draw(sprites_[i]);
			}

			if(bJoueur){
				while(bEnnemies && yEnnemie < yJoueur){
					app.draw((*it)->getSprite());
					++it;
					if(it == et) bEnnemies = false;
				}
				app.draw(Joueur.drawSprite());
			}

			while(bEnnemies){
					app.draw((*it)->getSprite());
					++it;
					if(it == et) bEnnemies = false;
			}

		}


	private:
		//Les types : 0 = Objet solide normal, 5 6 7 altar, 8 et 9 des gold, 10 escalier bas, 20 escalier haut,
		std::vector<int> type_;
		std::vector<int> y_;

		std::vector<sf::Sprite> sprites_;
		std::vector<sf::Vector2f> pos_;
		std::vector<sf::FloatRect> hitBoxs_;

		sf::Texture* texture_;

		std::unordered_map<int, int> yToIndex_;

		//Decors
		std::vector<sf::Sprite> spritesDeco_;

		//Objet animé
		std::vector<sf::Sprite> spriteAnimated_;
		std::vector<Animation> animation_;

		sf::Clock temps_;

		//Collectable
		std::vector<sf::Sprite> spriteColle_;
		std::vector<sf::FloatRect> hitBoxColle_;
		std::vector<int> typeColle_;
};






class Objet{

	public:
		Objet(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, bool const& solide = false, bool const& visible = true)
		: solide_(solide), visible_(visible), y_(pos.y), texture_(texture), sprite_(*texture_, posTextu) {
			sprite_.setPosition(pos);
		}

		virtual sf::Sprite& getSprite(){
			return sprite_;
		}


		virtual int ifCollision(Personnage* perso, float const& x, float const& y){
			return 0;
		}

		virtual sf::FloatRect getHitBox() const {
			return sprite_.getGlobalBounds();
		}

		int getY() const {
			return y_;
		}

		bool getSolide() const {
			return solide_;
		}

		bool getVisible() const {
			return visible_;
		}

	protected:
		bool solide_;
		bool visible_;
		int y_;

		sf::Texture* texture_;
		sf::Sprite sprite_;
};

class Table : public Objet {

	public:
		Table(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos)
		: Objet(texture, posTextu, pos, true), hitBox_(pos.x, pos.y + 7, 32, 14) {
			y_ = pos.y + 7;
		}

		virtual sf::FloatRect getHitBox() const{
			return hitBox_;
		}

	private:
		sf::FloatRect hitBox_;

};

class Chaise : public Objet {

	public:
		Chaise(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, bool const& sens_)
		: Objet(texture, posTextu, pos, true) {
			if(sens_){
				hitBox_ = sf::FloatRect(pos.x, pos.y + 11, 9, 7);
			}
			else{
				hitBox_ = sf::FloatRect(pos.x, pos.y + 7, 10, 7);
			}
		}

		virtual sf::FloatRect getHitBox() const{
			return hitBox_;
		}

	private:
		sf::FloatRect hitBox_;

};

class Conteneur : public Objet {

	public:
		Conteneur(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, bool const& type)
		: Objet(texture, posTextu, pos, true), type_(type) {
			if(type_){
				sprite_.setPosition(pos.x + 3, pos.y - 5);
				hitBox_ = sf::FloatRect(sprite_.getPosition().x, sprite_.getPosition().y+8, 10, 7); //tonneau
			}
			else{
				sprite_.setPosition(pos.x + 2, pos.y - 5);
				hitBox_ = sf::FloatRect(sprite_.getPosition().x, sprite_.getPosition().y+8, 12, 7); //caisse
			}
		}

		virtual sf::FloatRect getHitBox() const{
			return hitBox_;
		}


	private:
		bool type_; //true = tonneau, false = caisse

		sf::FloatRect hitBox_;

};

class Gold : public Objet {

	public:
		Gold(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, int const& nbGold) : Objet(texture, posTextu, pos, false, false) {}


	private:
		int nbGold_;
};

class Animated : public Objet {

	public:
		Animated(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, Animation const& anim, float const& tousLes)
		: Objet(texture, posTextu, pos, false), tousLes_(tousLes), anim_(anim) {
			temps_.restart();
		}

		virtual sf::Sprite& getSprite(){
			if(temps_.getElapsedTime().asSeconds() > tousLes_){
				sprite_.setTextureRect(anim_.nextSprite());
				temps_.restart();
			}
			return sprite_;
		}

	private:
		float tousLes_;
		Animation anim_;

		sf::Clock temps_;
};

class Altar : public Objet {

	public:
		Altar(sf::Texture* texture, sf::IntRect const& posTextu, sf::Vector2f const& pos, int const& type)
		: Objet(texture, posTextu, sf::Vector2f(pos.x, pos.y-20), true, true), type_(type), hitBox_(pos.x, pos.y+3, 16, 9) {
			y_ = pos.y;
		}

		virtual sf::FloatRect getHitBox() const{
			return hitBox_;
		}


	private:
		int type_;

		sf::FloatRect hitBox_;

};