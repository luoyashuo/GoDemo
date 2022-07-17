#include "boardstate.h"
boardState::boardState()
{

}
bool boardState::isPiece(int row, int col)
{
    if(this->graph[row][col]==boardState::GO_BLACK||this->graph[row][col]==boardState::GO_WHITE)
        return true;
    return false;
}

bool boardState::isSigned(int row, int col)
{
    if(this->tosettle[row][col]==boardState::GO_SELECTED_BLACK||this->tosettle[row][col]==boardState::GO_SELECTED_WHITE)
        return true;
    return false;
}

void boardState::GoQiCal(int row, int col,GoState color)
{
    if(flag[row][col]==1)
        return;
    flag[row][col]=1;
    if(this->graph[row][col]==boardState::GO_BLANK)
    {
        this->GoQiSum++;
        return;
    }
    if(this->graph[row][col]==boardState::GO_JIE_BLACKBAN)
    {
        this->GoQiSum++;
        return;
    }
    if(this->graph[row][col]==boardState::GO_JIE_WHITEBAN)
    {
        this->GoQiSum++;
        return;
    }
    if(this->graph[row][col]==boardState::GO_BOUNDARY)
    {
        return;
    }
    if(this->graph[row][col]==color)
    {
        for(auto i:{0,-1,1})
        {
            if(i==0)
            {
                for(auto j:{1,-1})
                     this->GoQiCal(row, j+col,color);
            }
            else
                 this->GoQiCal(i+row, col,color);
        }
    }
        return;
}
void boardState::GoQiUpdate()
{
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
        {
            if(this->isPiece(i,j))
            {
                this->GoQiInit();
                this->GoQiCal(i,j,this->graph[i][j]);
                this->GoQi[i][j]=this->GoQiSum;
            }
        }
}
void boardState::GoQiInit()
{
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
            this->flag[i][j]=0;
    this->GoQiSum=0;
}
bool boardState::isLegalToSettle(int row, int col, GoTurn turn)
{
    //边界不可落子
    if(this->graph[row][col]==boardState::GO_BOUNDARY)
        return false;
    //已有棋子处不可落子
    if(this->isPiece(row,col))
        return false;
    //劫处不可落子
    if(this->graph[row][col]==boardState::GO_JIE_BLACKBAN&&turn==boardState::GO_TURN_BLACK)
        return false;
    if(this->graph[row][col]==boardState::GO_JIE_WHITEBAN&&turn==boardState::GO_TURN_WHITE)
        return false;
    this->GoSettle(row,col,turn);
    this->GoQiUpdate();
    //没有造成提子效果且落子后该子的气为0
    if(this->GoQi[row][col]==0&&!this->isExisDiffQi0Piece(row,col,turn))
    {
        this->boardRepentance();
        return false;
    }
    this->boardRepentance();
    return true;
}
void boardState::GoSettle(int row, int col, GoTurn who)
{
    this->totalStep++;
    this->graph[row][col]=who==boardState::GO_TURN_BLACK?boardState::GO_BLACK:boardState::GO_WHITE;
    this->recordStack[this->totalStep].x=row;
    this->recordStack[this->totalStep].y=col;
    this->recordStack[this->totalStep].curState=who==boardState::GO_TURN_BLACK?boardState::GO_BLACK:boardState::GO_WHITE;
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
        {
            //注意，栈里面的提子数反而要再boardUpdate中维护
            this->replayStack[this->totalStep].graph[i][j]=this->graph[i][j];
        }
    this->preTurn=this->curTurn;
    this->curTurn=this->curTurn==boardState::GO_TURN_BLACK?boardState::GO_TURN_WHITE:boardState::GO_TURN_BLACK;
}
void boardState::boardUpdate()
{
    this->GoQiUpdate();
    //记录下上一步的位置和落子方
    boardState::GoTurn who=this->preTurn;
    int row=this->recordStack[this->totalStep].x;
    int col=this->recordStack[this->totalStep].y;
    boardState::GoTurn diffwho=who==boardState::GO_TURN_BLACK?boardState::GO_TURN_WHITE:boardState::GO_TURN_BLACK;
    boardState::GoState diffstate=boardState::GO_BLACK;
    boardState::GoState oldjie=boardState::GO_JIE_WHITEBAN;
    if(who==boardState::GO_TURN_BLACK)
    {
        diffstate=boardState::GO_WHITE;
        oldjie=boardState::GO_JIE_BLACKBAN;
    }//上一步走完之后，以前的劫都要变成blank
    //维护劫
    for(int i=1;i<=19;i++)
        for(int j=1;j<=19;j++)
        {
            if(this->graph[i][j]==oldjie)
            {
                this->graph[i][j]=boardState::GO_BLANK;
            }
        }
    //标记新产生的劫
    if(this->GoQi[row][col]==0&&!this->isExistSamePiece(row,col,who))
    {
        this->GoQiSign(row,col,who);
        for(auto i:{0,-1,1})
        {
            if(i==0)
            {
                for(auto j:{-1,1})
                {
                    if(this->GoQi[row][col+j]==0&&this->graph[row][col+j]==diffstate&&!this->isExistSamePiece(row,col+j,diffwho))
                    {
                        this->graph[row][col+j]=who==boardState::GO_TURN_BLACK?boardState::GO_JIE_WHITEBAN:boardState::GO_JIE_BLACKBAN;
                    }
                }
            }
            else
            {
                if(this->GoQi[row+i][col]==0&&this->graph[row+i][col]==diffstate&&!this->isExistSamePiece(row+i,col,diffwho))
                {
                    this->graph[row+i][col]=who==boardState::GO_TURN_BLACK?boardState::GO_JIE_WHITEBAN:boardState::GO_JIE_BLACKBAN;
                }
            }
        }
    }
    ///清空死子
    //先把气为零的子清空
    for(int i=1;i<=19;i++)
        for(int j=1;j<=19;j++)
        {
            if(this->graph[i][j]==diffstate&&this->GoQi[i][j]==0)
            {
                this->graph[i][j]=boardState::GO_BLANK;
                if(who==boardState::GO_TURN_BLACK)
                {
                    this->tiziBlack++;
                }
                else
                {
                    this->tiziWhite++;
                }
            }
        }
    //排除特殊情况：黑棋落子处同时杀死了两个以上的白子，即落子产生劫的同时还杀了白子，这就要将GO_JIE_WHITEBAN转化为GO_BLANK
    this->GoQiUpdate();
    if(this->GoQi[row][col]>=2)
    {
        for(auto i:{0,-1,1})
        {
            if(i==0)
            {
                for(auto j:{-1,1})
                {
                    if(this->graph[row][col+j]==(who==boardState::GO_TURN_BLACK?GoState::GO_JIE_WHITEBAN:GoState::GO_JIE_BLACKBAN))
                    {
                        this->graph[row][col+j]=GoState::GO_BLANK;
                                if(who==boardState::GO_TURN_BLACK)
                                {
                                    this->tiziBlack++;
                                }
                                else
                                {
                                    this->tiziWhite++;
                                }

                    }
                }
            }
            else
            {
                if(this->graph[row+i][col]==(who==boardState::GO_TURN_BLACK?GoState::GO_JIE_WHITEBAN:GoState::GO_JIE_BLACKBAN))
                {
                    this->graph[row+i][col]=GoState::GO_BLANK;
                            if(who==boardState::GO_TURN_BLACK)
                            {
                                this->tiziBlack++;
                            }
                            else
                            {
                                this->tiziWhite++;
                            }

                }
            }
        }
    }
    //再把栈更新
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
        {
            this->replayStack[this->totalStep].graph[i][j]=this->graph[i][j];
        }
    //维护replayStack中的提子数
    this->replayStack[this->totalStep].tiziBlack=this->tiziBlack;
    this->replayStack[this->totalStep].tiziWhite=this->tiziWhite;
}
void boardState::boardRepentance()
{
    this->totalStep--;
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
        {
            this->graph[i][j]=this->replayStack[this->totalStep].graph[i][j];
        }
    this->preTurn=this->curTurn;
    this->curTurn=this->curTurn==boardState::GO_TURN_BLACK?boardState::GO_TURN_WHITE:boardState::GO_TURN_BLACK;
    //维护提子
    this->tiziBlack=this->replayStack[this->totalStep].tiziBlack;
    this->tiziWhite=this->replayStack[this->totalStep].tiziWhite;
}
bool boardState::isExistDiffPiece(int row, int col, GoTurn turn)
{
    boardState::GoState diff=turn==boardState::GO_TURN_BLACK?boardState::GO_WHITE:boardState::GO_BLACK;
    for(auto i:{0,-1,1})
    {
        if(i==0)
        {
            for(auto j:{-1,1})
            {
                if(this->graph[row][col+j]==diff)
                    return true;
            }
        }
        else
        {
            if(this->graph[row+i][col]==diff)
                return true;
        }
    }
    return false;
}
bool boardState::isExisDiffQi0Piece(int row, int col, GoTurn turn)
{
    boardState::GoState diff=turn==boardState::GO_TURN_BLACK?boardState::GO_WHITE:boardState::GO_BLACK;
    this->GoQiUpdate();
    for(auto i:{0,-1,1})
    {
        if(i==0)
        {
            for(auto j:{-1,1})
            {
                if(this->graph[row][col+j]==diff)
                {

                    if(this->GoQi[row][col+j]==0)
                        return true;
                }
            }
        }
        else
        {
            if(this->graph[row+i][col]==diff)
            {

                if(this->GoQi[row+i][col]==0)
                    return true;
            }
        }
    }
    return false;
}
void boardState::GoQiSign(int row, int col, GoTurn turn)
{
    boardState::GoState same=turn==boardState::GO_TURN_WHITE?boardState::GO_WHITE:boardState::GO_BLACK;
    this->GoQi[row][col]=-1;
    for(auto i:{0,-1,1})
    {
        if(i==0)
        {
            for(auto j:{-1,1})
            {
                if(this->GoQi[row][col+j]==0&&this->graph[row][col+j]==same)
                {
                    this->GoQiSign(row,col+j,turn);
                }
            }
        }
        else
        {
            if(this->GoQi[row+i][col]==0&&this->graph[row+i][col]==same)
            {
                this->GoQiSign(row+i,col,turn);
            }
        }
    }
}
bool boardState::isExistSamePiece(int row, int col, GoTurn turn)
{
    boardState::GoState same=turn==boardState::GO_TURN_WHITE?boardState::GO_WHITE:boardState::GO_BLACK;
    for(auto i:{0,-1,1})
    {
        if(i==0)
        {
            for(auto j:{-1,1})
            {
                if(this->graph[row][col+j]==same)
                    return true;
            }
        }
        else
        {
            if(this->graph[row+i][col]==same)
                return true;
        }
    }
    return false;
}

void boardState::initRangzi(int n)
{
    switch (n) {
    case 0:{break;}
    case 2:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        break;
    }
    case 3:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        break;
    }
    case 4:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        break;
    }
    case 5:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        break;
    }
    case 6:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        this->graph[10][4]=boardState::GO_BLACK;
        break;
    }
    case 7:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        this->graph[10][4]=boardState::GO_BLACK;
        this->graph[16][10]=boardState::GO_BLACK;
        break;
    }
    case 8:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        this->graph[10][4]=boardState::GO_BLACK;
        this->graph[16][10]=boardState::GO_BLACK;
        this->graph[10][16]=boardState::GO_BLACK;
        break;
    }
    case 9:{
        this->graph[4][4]=boardState::GO_BLACK;
        this->graph[16][16]=boardState::GO_BLACK;
        this->graph[16][4]=boardState::GO_BLACK;
        this->graph[4][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        this->graph[10][4]=boardState::GO_BLACK;
        this->graph[16][10]=boardState::GO_BLACK;
        this->graph[10][16]=boardState::GO_BLACK;
        this->graph[10][10]=boardState::GO_BLACK;
        break;
    }
    }
}

void boardState::MuUpdate()
{
    this->muBlack=this->muWhite=0;
    for(int i=0;i<21;i++)
        for(int j=0;j<21;j++)
        {
            if(this->signGraph[i][j]==boardState::GO_SIGN_BLACK)
                this->muBlack++;
            else if(this->signGraph[i][j]==boardState::GO_SIGN_WHITE)
                this->muWhite++;
        }
}

void boardState::boardStopOneStep()
{
    boardState::GoTurn p;
    p=this->preTurn;
    this->preTurn=this->curTurn;
    this->curTurn=p;
}

void boardState::stopDeducation(int step)
{
    if(this->stopStep!=-1)
    {
        this->totalStep=this->stopStep;
        for(int i=1;i<=19;i++)
            for(int j=1;j<=19;j++)
            {
                this->graph[i][j]=this->replayStack[this->stopStep].graph[i][j];
                this->tiziBlack=this->replayStack[this->stopStep].tiziBlack;
                this->tiziWhite=this->replayStack[this->stopStep].tiziWhite;
            }
    }
}

void boardState::initBoard(int rangzi)
{
    for(int i=1;i<20;i++)
        for(int j=1;j<20;j++)
        {
            this->graph[i][j]=GoState{GO_BLANK};
            this->GoQi[i][j]=0;
            this->flag[i][j]=0;
            this->replayStack[0].graph[i][j]=GoState{GO_BLANK};
            this->signGraph[i][j]=GoSign{GO_SIGN_BLANK};
        }
    this->curTurn=GoTurn{GO_TURN_BLACK};
    this->preTurn=GoTurn{GO_TURN_WHITE};
    this->initRangzi(rangzi);
    if(rangzi>=1)
    {
        this->preTurn=GoTurn{GO_TURN_BLACK};
        this->curTurn=GoTurn{GO_TURN_WHITE};
    }
    for(int i=1;i<20;i++)
        for(int j=1;j<20;j++)
        {
            this->replayStack[0].graph[i][j]=this->graph[i][j];
        }
    for(int j=0;j<21;j++)
    {
        this->graph[0][j]=GoState{GO_BOUNDARY};
        this->graph[j][0]=GoState{GO_BOUNDARY};
        this->graph[20][j]=GoState{GO_BOUNDARY};
        this->graph[j][20]=GoState{GO_BOUNDARY};
        this->GoQi[20][j]=0;
        this->GoQi[j][20]=0;
        this->GoQi[j][0]=0;
        this->GoQi[0][j]=0;
        this->flag[20][j]=0;
        this->flag[0][j]=0;
        this->flag[j][20]=0;
        this->flag[j][0]=0;
        this->replayStack[0].graph[20][j]=GoState{GO_BOUNDARY};
        this->replayStack[0].graph[j][20]=GoState{GO_BOUNDARY};
        this->replayStack[0].graph[0][j]=GoState{GO_BOUNDARY};
        this->replayStack[0].graph[j][0]=GoState{GO_BOUNDARY};
        this->signGraph[j][0]=GoSign{GO_SIGN_BLANK};
        this->signGraph[20][j]=GoSign{GO_SIGN_BLANK};
        this->signGraph[j][20]=GoSign{GO_SIGN_BLANK};
        this->signGraph[0][j]=GoSign{GO_SIGN_BLANK};
    }
    this->recordStack[0].x=0;
    this->recordStack[0].y=0;
    this->recordStack[0].curState=boardState::GO_BOUNDARY;
    this->totalStep=0;//初始步数设定为0
    this->GoQiSum=0;
    this->muBlack=this->muWhite=0;
}
