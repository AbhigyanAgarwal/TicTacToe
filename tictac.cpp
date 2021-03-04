#include<bits/stdc++.h>
#define pb push_back
using namespace std;
vector<vector<int>> curr_state(3,vector<int> (3)); // game board
int total_calculations=0;
void clear_game() // to clear the board
{
    for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                curr_state[i][j]=0;
        }   
    return ;
}
int check(vector<vector<int>> state) // to check the current state if any player has won
{
    for(int i=0;i<3;i++)
        {
            if(state[i][1]==state[i][2] && state[i][0]==state[i][1] && state[i][0]!=0)
                return state[i][0];
            if(state[1][i]==state[2][i] && state[1][i]==state[0][i] && state[0][i]!=0)
                return state[1][i];
        }
    if(state[0][0]==state[1][1] && state[2][2]==state[1][1] && state[0][0]!=0)
        return state[0][0];
    if(state[1][1]==state[0][2] && state[1][1]==state[2][0] && state[1][1]!=0)
        return state[1][1];
    for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                if(state[i][j]==0)
                    return 2;
    
        return 0;
}
void display_state() // to print the board
{
    for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                {
                    if(curr_state[i][j]==0)
                        cout<<"   ";
                    else{
                        cout<<" "<<((curr_state[i][j]==1)?'X':'O')<<" ";
                    }
                    if(j!=2)
                        cout<<"|";     
                }
                if(i!=2)
                    {cout<<"\n";
                    cout<<"___|___|___\n";
                    }
        }
        cout<<endl;
}
int find_win_move(vector<vector<int>> &temp,int player) // alphabeta pruning to find optimal moves for the computer
{
    total_calculations++;
    int i,j,mx;
    int f=check(temp);
    if(f!=2) // to check if the game is won by any side 
        return f;

    if(player)  // to check whether it is the computers move or user so that max or min what is desireable can be determined
    { // computers move to find the max possible score for the computer among all moves
        mx=-2; // since we want max so we intilize it with value less than minimum
        for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                    {
                        if(temp[i][j]==0) // checking if the move is legal
                            {
                                temp[i][j]=1; // changing state
                                if(mx==-2)
                                    mx=find_win_move(temp,player^1);
                                else
                                    mx=max(mx,find_win_move(temp,player^1)); // max move for computer  xor to change the player of the next move 1^1=0 and 0^1 =1
                                temp[i][j]=0; // changing state
                            }
                    }
            }
    } 
    else
    {  // when checking for min possoble outcome among all possible moves of player 2 
        mx=2; // since we want min so we initialie with max+1
        for(i=0;i<3;i++)
            {
                for(j=0;j<3;j++)
                    {
                        if(temp[i][j]==0)
                            {
                                temp[i][j]=-1; // changing state
                                if(mx==2)
                                    mx=find_win_move(temp,player^1);
                                else
                                    mx=min(mx,find_win_move(temp,player^1)); // xor to change the player of the next move 1^1=0 and 0^1 =1
                                temp[i][j]=0; // changing state
                            }
                    }
            }
        
    }
    //cout<<mx<<" "<<player<<endl;
    return mx;
       
}
void comp_move() // to generate all possible moves and then select randomly one move among the best moves
{ 
    vector<pair<int,int>> ans;
    int mx=-2,i,j,x;
    vector<vector<int>> temp(3,vector<int> (3));
    temp=curr_state;
    for(i=0;i<3;i++)
        {
            for(j=0;j<3;j++)
                {
                    if(curr_state[i][j]==0) // to check if move is legal and find if it is optimal
                        {
                            temp[i][j]=1;
                            x=find_win_move(temp,0);
                            temp=curr_state;
                            if(x>mx)
                                {
                                    ans.clear();
                                    ans.pb({i,j});
                                    mx=x;
                                }
                            else if(x==mx)
                                {
                                    ans.pb({i,j});
                                }
                        }
                }
        }
    
    i= rand()%((int)ans.size()); // randomly select one of the optimal moves
    curr_state[ans[i].first][ans[i].second]=1;
    return ;

}
int legal(int x, int y) // checking if users move is legal or not
{
    if(x>3 || x<1 || y>3 || y<1 )
        return 0;
    if(curr_state[x-1][y-1]!=0)
        return 0;
    else
    {
        return 1;
    }
    
}
void user_move() // making users move 
{
    display_state(); // to display the current board to the user
    cout<<endl;
    int x,y,f=1;
    while(f)
    {
        cout<<"Make your move, enter the coordinates of the position you choose, as X Y"<<endl;
        cin>>x>>y;
        if(legal(x,y)) // check legal
            {curr_state[x-1][y-1]=-1;return ;} // make the move if legal
        else
        {
            cout<<"Illegal move please try again"<<endl;
        }
        
    }
    return ;
}
void move(int x) // deciding whose move it is and sending control to that function
{
    if(x==1)
        comp_move();
    else
    {
        user_move();
    }
    return ;
    
}
int start_game(int first) // to begin a match
{
    int c=0,f=1;
    while(f && c<9)
    {
        c++; // to count moves 
        if(c&1) // to decide whose move it is, the player that started will always play at odd moves
            move(first);
        else
        {
            move(first^1); 
        }
        int f=check(curr_state); // check if the game is over any playerhas won or not
        if(f!=2)
            return f; // return winning side
        f=1;        
    }
    return 0; // if  the match is tie
}
void solve()
{
    int c=0,f=1,x;
    while(f)
    {
        clear_game(); // clear the board
        c++; // count the no of matches played, so that alternately first move can be given to user or computer
        int d=start_game(c&1);
        if(d==1)
            cout<<"COMPUTER WINS";
        else if(d==-1)
            cout<<"PLAYER WINS";
        else
            cout<<"TIE MATCH";
        
        cout<<endl<<endl;
        display_state(); // display end state of the game
        cout<<endl;
        cout<<"TO EXIT press 0 else press 1 to PLAY AGAIN"<<endl;
        cin>>x;
        if(x==0)
            break;
    }
}
int main()
{
        srand(time(NULL)); // to generate random and different values everytime
        solve();
        cout<<total_calculations<<endl;
        return 0;
}