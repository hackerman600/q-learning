#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <memory> 
#include <any>

class Food {

    public:
        std::vector<Food> food_vector;
        sf::RectangleShape food;
        std::random_device rd;
        float x;
        float y;
        
        Food(float ex, float wy){
            food.setSize(sf::Vector2f(10.f,10.f));
            food.setFillColor(sf::Color::Blue);
            food.setPosition(sf::Vector2f(ex,wy));
            x = ex;
            y = wy;

        }

        
        void move(){ 
            x += 2.5; //was 0.25
            //std::cout << "x is: " << x << std::endl;
            sf::Vector2f new_position = food.getPosition() + sf::Vector2f(20.0,0.f); //sf::Vector2f was 2.0 
            food.setPosition(new_position);

        }

        void draw(sf::RenderWindow& window){
            //std::cout << "draw x is: " << x << std::endl;
            window.draw(food);
        }


};


class agent{

    private:
           std::random_device rd;

    
    public:

        int agent_w = 20;
        int agent_h = 60;
        int shoot = 0;
        int choice;
        int max_ind;
        float val0;
        float val1;
        float val2;

        std::vector<std::vector<std::any>> q_table;
        static std::vector<float> gen_x_y(){
            std::vector<float> xy;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int>ex(0,1);
            std::uniform_int_distribution<int>why(0,60);
            int xx = ex(gen);
            float y = 400.f; 
            //float y = why(gen) * 10;
            //std::cout << "y in bird is: " << y << std::endl;
            float x = static_cast<float>(xx); 
            xy.push_back(x);
            xy.push_back(y);
            return xy;
        }

        std::vector<float> pos_xy = gen_x_y();
        float x = static_cast<float>(pos_xy[0]);
        float y = static_cast<float>(pos_xy[1]);
        sf::RectangleShape age;

        void create_q_table(){
        
            for (int agent_y = 0; agent_y <= 600; agent_y+=10){
                for (int agent_x = 0; agent_x < 2; agent_y++){
                    std::vector<std::any> val1 = {0.f, 0.f, 0.f,agent_y, agent_x};
                    q_table.push_back(val1);

                }

            }    
        }

        
        
        agent(){
        age.setSize(sf::Vector2f(20.f,60.f));
        age.setPosition(sf::Vector2f(x,y));
        age.setFillColor(sf::Color::Black);
        
        }
        

        void draw(sf::RenderWindow& window){
            window.draw(age);
          
        }

        void shooty(){
            shoot = 1; 
        }

        void move(sf::Vector2f val){
            x -= val.x;
            y -= val.y; 
            sf::Vector2f new_position = age.getPosition() - val; 
            age.setPosition(new_position);
            
        }

        void decide(std::vector<std::any> v1, float epsilon){
            std::mt19937 gen(rd());
            std::default_random_engine engine(rd());
            std::uniform_real_distribution<float>ep(0.f,1.f);
            std::uniform_int_distribution<int>iinntt(0,2);
            
            if(ep(gen) < epsilon){
                choice = iinntt(engine);
            }

            else {
                compare_qvalues(v1);
            }

            
               
        } 


        void compare_qvalues(std::vector<std::any> vec1){
            val0 = std::any_cast<float>(vec1[0]);
            val1 = std::any_cast<float>(vec1[1]);
            val2 = std::any_cast<float>(vec1[2]);

            if (val0 > val1 || val0 > val2){                
                if (val0 > val2){
                    if (val0 > val1){
                        max_ind = 0;
                    }     

                    if (val0 < val1){
                        max_ind = 1;
                    }    

                    else {
                        max_ind = 0;
                    }          
                } 


                else if (val0 > val1){
                    if (val0 < val2){
                        max_ind = 2;
                    }

                    if (val0 == val2){
                        max_ind = 0;
                    }
                }

            } 
            

            else if (val1 > val0 || val1 > val2){
                
                if (val1 > val0){
                    if (val1 > val2){
                        max_ind = 1;
                    }     

                    if (val1 < val2){
                        max_ind = 2;
                    }    

                    else {
                        max_ind = 1;
                    }          
                } 


                else if (val1 > val2){
                    if (val1 < val0){
                        max_ind = 0;
                    }

                    if (val1 == val0){
                        max_ind = 1;
                    }
                }               

            }


            else if (val2 > val0 || val2 > val1){
                if (val2 > val0){
                    if (val2 > val1){
                        max_ind = 2;
                    }     

                    if (val2 < val1){
                        max_ind = 1;
                    }    

                    else {
                        max_ind = 2;
                    }          
                } 


                else if (val2 > val1){
                    if (val2 < val0){
                        max_ind = 0;
                    }

                    if (val0 == val2){
                        max_ind = 2;
                    }
                }

            }

            else {
                max_ind = 0;
            }
            
            choice = max_ind;
            
        }    


        void reset(){
            pos_xy = gen_x_y();
            x = static_cast<float>(pos_xy[0]);
            y = static_cast<float>(pos_xy[1]);
            age.setPosition(sf::Vector2f(x,y));
            shoot = 0;

        }

}; 

int main(){
    std::vector<std::vector<std::vector<std::any>>> q_table;
    std::vector<std::any> cur_q_table;
    std::vector<std::any> next_q_table;
    agent ag;

    for (int y = 0; y <= 600; y+=10){
        std::vector<std::vector<std::any>> q = {};
        q_table.push_back(q);
        for (int x = 0; x < 2; x++){
            std::vector<std::any> val0 = {0.f,0.f,0.f,y, x};
            q_table.back().push_back(val0);
        }

    }

    std::cout << "size of q_table is: " << q_table.size() << std::endl;
    sf::RenderWindow window(sf::VideoMode(600,600), "Shooting game");
    sf::RectangleShape target(sf::Vector2f(30.f,30.f));
    std::vector<std::unique_ptr<Food>> food_vector;
    target.setFillColor(sf::Color::Red);
    target.setPosition(570,150);
    sf::Text text;
    sf::Text text2;
    sf::Text epsilon_text;
    sf::Font font;
    float maximus;
    int destroy;
    int shoot;
    float im_reward = 1.f;
    int max_next_index; 
    float score = 0.f;
    float epsilon = 1.f;
    float alpha = 1.f;
    float gamma = 0.5f; 
    float prev_score = score;
    float max_next; 
    float q_val;
    int agy;
    int agx;
    

    while (window.isOpen())

    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K){
                ag.move(sf::Vector2f(0.f,10.f));
                score -= 0.5f; 
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M){
                ag.move(sf::Vector2f(0.f,-10.f));
                score -= 0.5f; 
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space){    
                std::cout << "ag.y is: " << ag.y << std::endl;             
                food_vector.push_back(std::make_unique<Food>(ag.x+20.f , ag.y + 25.f));
                ag.shooty();
                //std::cout << "size of food_vector is: " << food_vector.size();

            }    

        }


        if (!font.loadFromFile("/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf")){
            throw std::runtime_error("font failed to load");
        }

        text.setString("Score: " + std::to_string(score));
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Red);
        text.setFont(font);
        text.setPosition(20,1);
        text2.setString("Prev-Score: " + std::to_string(prev_score));
        text2.setCharacterSize(24);
        text2.setFillColor(sf::Color::Blue);
        text2.setFont(font);
        text2.setPosition(315,1);
        epsilon_text.setString("Epsilon: " + std::to_string(epsilon));
        epsilon_text.setCharacterSize(16);
        epsilon_text.setFillColor(sf::Color::Black);
        epsilon_text.setFont(font);
        epsilon_text.setPosition(20,35);
        window.clear(sf::Color::White);
        window.draw(target);
        window.draw(text);
        window.draw(text2);
        window.draw(epsilon_text);
        ag.draw(window);


        if (food_vector.size() > 0){
            //std::cout << "size of food_vector is: " << food_vector.size();
            ag.shoot = 1;
            for (int c = 0; c < food_vector.size(); c++){
                food_vector[c]->draw(window);
                food_vector[c]->move();
                //std::cout << "food_vector[c]->x: " << food_vector[c]->x;
                if(food_vector[c]->x > 90.f){
                    destroy = 1;
                    prev_score = score;
                    score = 0.f;
                    ag.shoot = 0;
                     
                }    
                    
            }
        }

        if (destroy == 1){
            food_vector.clear();
            ag.reset();
            destroy = 0;
        }


        //SET & CHECK STATE IS CORRECT

        agy = static_cast<int>(ag.y);
        agx = static_cast<int>(ag.x);
        //std::cout << "agent_x is: " << agx << std::endl;
        //std::cout <<  "agent_y is: " << agy/10 << std::endl;
                
        cur_q_table = q_table[agy/10][agx];
        //std::cout <<  "cur_q_table.size() is: " << cur_q_table.size() << std::endl;

        /*float val0 = std::any_cast<float>(cur_q_table[0]);
        std::cout << "val0 = " << val0 << std::endl;
        float val1 = std::any_cast<float>(cur_q_table[1]);
        std::cout << "val1 = " << val1 << std::endl;
        float val2 = std::any_cast<float>(cur_q_table[2]);
        std::cout << "val2 = " << val2 << std::endl;*/
        int val3 = std::any_cast<int>(cur_q_table[3]);
        //std::cout << "val3 = " << val3 << std::endl;
        int val4 = std::any_cast<int>(cur_q_table[4]);
        //std::cout << "val4 = " << val4 << std::endl;

        if (val3 != agy || val4 != agx){
            std::cout << "table indexing is wrong. Check what is wrong with it." << std::endl;
            std::cout <<  "agent_y is: " << agy << std::endl;
            std::cout << "val3 = " << val3 << std::endl;
            std::cout << "agent_x is: " << agx << std::endl;
            std::cout << "val4 = " << val4 << std::endl;
            break;
        }

        
        //TAKE ACTION / DECIDE
        if (ag.shoot != 1){
            ag.decide(cur_q_table, epsilon);

            if (ag.choice == 0){
                ag.move(sf::Vector2f(0.f,10.f));
                if (ag.y < 0){
                    im_reward = -100.f;
                    score -= 100.f; 
                    ag.reset();
                } else {
                    im_reward = -1.f;
                    score -= 1.f; 
                }
            } else if (ag.choice == 1){
                ag.move(sf::Vector2f(0.f,-10.f));
                if (ag.y + ag.agent_h > 600){
                    im_reward = -100.f;
                    score -= 100.f; 
                    ag.reset();
                } else {
                    im_reward = -1.f;
                    score -= 1.f; 
                }

            } else if (ag.choice == 2){
                ag.shooty();
                im_reward = -1.f;
                score -= 1.f; 
                food_vector.push_back(std::make_unique<Food>(ag.x + 20.f, ag.y + 25.f));
                if (ag.y + 25.f >= 150 && ag.y + 25.f <= 180){
                    im_reward += 100;
                    score += 100.f; 
                } else if (ag.y + 25.f < 150 || ag.y + 25.f > 180){
                    im_reward -= 100;   
                    score -= 100.f;  
                }
            }
        
        

        //SET NEXT STATE
        int agy_n = static_cast<int>(ag.y);
        int agx_n = static_cast<int>(ag.x);
        next_q_table = q_table[agy/10][agx];
        float n_val0 = std::any_cast<float>(next_q_table[0]);
        float n_val1 = std::any_cast<float>(next_q_table[1]);
        float n_val2 = std::any_cast<float>(next_q_table[2]);


        //CALCULATE Q SCORE AND UPDATE TABLE
        if (n_val0 > n_val1 || n_val0 > n_val2){                
                if (n_val0 > n_val2){
                    if (n_val0 > n_val1){
                        max_next_index = 0;
                    }     

                    if (n_val0 < n_val1){
                        max_next_index = 1;
                    }    

                    else {
                        max_next_index = 0;
                    }          
                } 


                else if (n_val0 > n_val1){
                    if (n_val0 < n_val2){
                        max_next_index = 2;
                    }

                    if (n_val0 == n_val2){
                        max_next_index = 0;
                    }
                }

            } 
            

        else if (n_val1 > n_val0 || n_val1 > n_val2){
                
                if (n_val1 > n_val0){
                    if (n_val1 > n_val2){
                        max_next_index = 1;
                    }     

                    if (n_val1 < n_val2){
                        max_next_index = 2;
                    }    

                    else {
                        max_next_index = 1;
                    }          
                } 


                else if (n_val1 > n_val2){
                    if (n_val1 < n_val0){
                        max_next_index = 0;
                    }

                    if (n_val1 == n_val0){
                        max_next_index = 1;
                    }
                }               

            }


        else if (n_val2 > n_val0 || n_val2 > n_val1){
                if (n_val2 > n_val0){
                    if (n_val2 > n_val1){
                        max_next_index = 2;
                    }     

                    if (n_val2 < n_val1){
                        max_next_index = 1;
                    }    

                    else {
                        max_next_index = 2;
                    }          
                } 


                else if (n_val2 > n_val1){
                    if (n_val2 < n_val0){
                        max_next_index = 0;
                    }

                    if (n_val0 == n_val2){
                        max_next_index = 2;
                    }
                }

            }


        else {
                max_next_index = 0;
            }


        max_next = std::any_cast<float>(next_q_table[max_next_index]);
        //new_q_value = cur_val + alpha * (im_reward + (gamma * next_max)) - cur_val;
        q_val = std::any_cast<float>(cur_q_table[ag.choice]) + alpha * (im_reward + (gamma * max_next)) - std::any_cast<float>(cur_q_table[ag.choice]);
        q_table[agy/10][agx][ag.choice] = q_val;
        
        if (epsilon <= 0){
            epsilon = 0;
        }
        else{
            epsilon -= 0.00002;//0.00002
        }
        
        }

        

        //std::cout << "ag.choice is: " << ag.choice;



    window.display();
    }

}
