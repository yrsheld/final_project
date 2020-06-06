#pragma once

#include <UltraOOXX/Wrapper/AI.h>
#include <UltraOOXX/UltraBoard.h>
#include <algorithm>
#include <random>
#include <ctime>
 
class AI : public AIInterface
{
//add private members(06/05)
private:
	std::pair<int,int> Board_ID;
	
	//find legal places in a single "Board"
	void find_legal(Board& target,std::vector<std::pair<int,int>>& L,int& x,int& y){  
    	for(int i=0;i<3;i++){
    		for(int j=0;j<3;j++){
    			if(target.state(i,j)==BoardInterface::Tag::None){
    				L.push_back(make_pair(i,j));
				}
			}
		}
		int r=rand()%(L.size());
		x=L[r].first; y=L[r].second;    
	}
	
public:
    void init(bool order) override
    {
    	order=true;
        // any way  
	}
    void callbackReportEnemy(int x, int y) override
    {
    	Board_ID.first=x/3; Board_ID.second=y/3;        
        // give last step, so that the enemy know which "board" to go
    }
    
    std::pair<int,int> queryWhereToPut(TA::UltraBoard U) override
    {     //generate the coordinate (make next move)
        
    	int x,y;  //the decision made
    	int BX=Board_ID.first, BY=Board_ID.second;   //the last coordinate of enemy = the current board ID
		Board& target = U.sub(BX,BY);
		std::vector<std::pair<int,int>> OK_Boards;
		std::vector<std::pair<int,int>> OK_Tags;
		
		//if the board is full 
		if(target.full()){     //search other available boards
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if(!U.sub(i,j).full()){
						OK_Boards.push_back(make_pair(i,j));    
					}
				}
			}
			//(perhaps don't need to deal with the all-full scenario (?)
			int r=rand()%(OK_Boards.size());  //choose the board  
			find_legal(U.sub(OK_Boards[r].first,OK_Boards[r].second),OK_Tags,x,y);
			x+=(OK_Boards[r].first)*3; 
			y+=(OK_Boards[r].second)*3;
		}
       	else{
       		find_legal(target,OK_Tags,x,y);
			x+=BX*3; y+=BY*3;	
		}
		return std::make_pair(x,y);
    }
};
