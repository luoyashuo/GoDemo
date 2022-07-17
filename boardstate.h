#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include <QObject>
#include<QSound>
#include<QMediaPlayer>
/*可能添加的功能有
 * 1.统计某一方的所有劫材
 * 2.给不同的官子进行标记和排序
*/
class boardState : public QObject
{
    Q_OBJECT
/////////////////////////////以下功用于实现基本规则//////////////////////////////////
public:
    typedef enum _goselect{
        GO_SELECTED_NULL,//未被选中
        GO_SELECTED_WHITE,//马上要落下黑子
        GO_SELECTED_BLACK,//马上要落下白子
    }GoSelect;
    typedef enum _gostate{
        GO_BOUNDARY,//棋盘的边界
        GO_BLACK,
        GO_WHITE,
        GO_BLANK,
        GO_JIE_BLACKBAN,//黑棋禁止下的劫
        GO_JIE_WHITEBAN,//白棋禁止下的劫
    }GoState;
    //记录该谁落子了
    typedef enum _goturn{
        GO_TURN_BLACK,
        GO_TURN_WHITE,
        GO_TURN_BAN,
    }GoTurn;
    int GoQi[21][21];//用来存储每个节点的气的多少，如果为0，就要提子,-1是特殊标记
    typedef struct _gostack{
        int x,y;
        GoState curState;
    }GoStack;
    typedef struct _goreplay{
        GoState graph[21][21];
        int tiziBlack=0;
        int tiziWhite=0;
    } GoReplay;
public:
        QSound *music;
    GoState graph[21][21];//记录棋盘的信息
    GoSelect tosettle[21][21];//用来判断交叉点是否被选中
    int totalStep=0;//存储总步数，同时也是维护replayStack recordStack的栈顶，//初始步数设定为0
    GoReplay replayStack[500];//用来复盘的栈,存储的是每一步之后棋盘的模样
    GoStack recordStack[500];//用来记录每一步的位置和颜色
    GoTurn curTurn;//当前是该谁下了
    GoTurn preTurn;//前一步是谁下的
    int GoQiSum;//帮助计算某个坐标的气
    int flag[21][21];//作用同上，用来深度优先遍历，0表示未被访问过
public:
    boardState();
    void initBoard(int rangzi);//用来初始化棋盘
    bool isPiece(int row,int col);//用来判断某个交叉点的位置是不是棋子，是ture否false
    bool isSigned(int row,int col);//用来判断某个交叉点是否被标记了
    void  GoQiCal(int row,int col,GoState color);//计算某个交叉点当前的气,结果返回到int GoQiSum//从当前坐标开始进行深度优先遍历,在当前坐标有棋子的时候再调用//在调用前要将GoQiSum初始化为0，以及flag初始化为0
    void  GoQiUpdate();//用来更新棋盘上每个位置的气
    void  GoQiInit();//用来更新flag和GoQiSum
    bool  isLegalToSettle(int row,int col,GoTurn turn);//判断turn落子后，棋子是否可以在当前位置落下
    void  GoSettle(int row,int col,GoTurn who);//who在row，col处落子
    void boardUpdate();//落子之后更新棋盘
    void boardRepentance();//悔棋一步
    bool isExistDiffPiece(int row,int col,GoTurn turn);//判断row,col处四周是不是存在与turn方不同颜色的棋子
    bool isExisDiffQi0Piece(int row,int col,GoTurn turn);//判断row,col处四周是不是存在与turn方不同颜色的棋子
    void GoQiSign(int row,int col,GoTurn turn);//用来将与row，col处相邻的棋子全部标记为-1，来避免被提子
    bool isExistSamePiece(int row,int col,GoTurn turn);//判断与row,col处四周是不是存在与turn方相同颜色的棋子
    void initRangzi(int n);//更新让了子之后的棋盘状态
signals:
/////////////////////////////以下用于实现标记点目，数子,交互功能//////////////////////////////////
public:
    typedef enum _gosign{
        GO_SIGN_BLANK,
        GO_SIGN_BLACK,
        GO_SIGN_WHITE,
    }GoSign;
    GoSign signGraph[21][21];
    int GoSignSum=0;
    int muWhite=0;
    int muBlack=0;//黑棋和白棋的目数
    int tiziBlack=0;//黑棋提子数
    int tiziWhite=0;//白棋的提子数
    int whoWin=0;//记录再JUDGE模式下谁赢了，0：NULL,1:BLACKWIN,2:WHITEWIN
    void MuUpdate(); //更新黑白棋的目数
public:
    void boardStopOneStep();//停一手
//////////////////////////////以下用于推演//////////////////////////////////
public:
    int stopStep=-1;//保存开始推演时候的步数,-1表示已经推出了推演状态
    void stopDeducation(int step);//返回停棋时候的状态
};

#endif // BOARDSTATE_H
