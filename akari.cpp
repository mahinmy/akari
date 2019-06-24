#include<iostream>
#include<string>
using namespace std;

struct Point{
    int x;
    int y;
};
template <class T>
struct Node{
    T data;
    Node<T> *next;
};

template <class T>
class LinkStack{
private:
    Node<T> *top;
public:
    LinkStack(){
        top = NULL;
    }
    ~LinkStack(){
        Node<T> *tmp = NULL;
        while(top!=NULL){
            tmp = top;
            top = top->next;
            delete tmp;
        }
    }
    void PrintStack(){
        Node<T> *tmp = NULL;
        tmp = top;
        cout << "Top of stack: ";
        while(tmp!=NULL){
            cout << tmp->data << " ";
            tmp = tmp->next;
        }
        cout << "Bottom of stack" << endl;
    }
    void Push(T x){
        Node<T> *s = NULL;
        s = new Node<T>;
        s->data = x;
        s->next = top;
        top = s;
    }
    T Pop(){
        Node<T> *tmp = NULL;
        if(top==NULL) throw "Empty";
        tmp = top;
        top = top->next;
        T x = tmp->data;
        delete tmp;
        return x;
    }
    T GetTop(){
        if(top!=NULL) return top->data;
        else throw "Empty";
    }
    bool IsEmpty(){
        return (top==NULL);
    }
};
class Akari{
    private:
        int row,column;
        int puzzle_list[100][100];
        int mode;
        LinkStack<Point*>*s1;
        LinkStack<Point*>*s2;
        bool hasPrinted;
        bool judge=0;
        int colSplit[100][100];//列分块
        int rowSplit[100][100];//行分块
        bool colAva[100][100];
        bool rowAva[100][100];
        int maxColumn;
        int maxRow;
        int numColumn[100];
        int numRow[100];
    public:
        Akari(int **sudoku, int r, int c, int input_mode=1){
            row = r;
            column = c;
            mode = input_mode;
            for(int i=0;i<r;i++){
                for(int j = 0;j<c;j++){
                    puzzle_list[i][j]=sudoku[i][j];
                }
            }
            s1 = new LinkStack<Point*>();
            s2 = new LinkStack<Point*>();
            hasPrinted=false;
        }
        void init(){
            //列分块
            for(int j=0;j<column;j++){
                int num=0;
                for(int i=0;i<row;i++){
                    if(i==0){
                        while(puzzle_list[i][j]>=-1){
                            i=i+1;
                        }
                    }
                    if(puzzle_list[i][j]>=-1){
                        num+=1;
                        while(i+1<row&&puzzle_list[i+1][j]>=-1){
                            i=i+1;
                        }
                    }
                    if(puzzle_list[i][j]==-2){
                        colSplit[i][j]=num;
                    }
                }
                numColumn[j]=num;
            }
            for(int i=0;i<column;i++){
                for(int j=0;j<numColumn[i];j++){
                    colAva[i][j]=false;
                }
            }
            for(int i=0;i<row;i++){
                for(int j=0;j<column;j++){
                    cout<<colSplit[i][j]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
            //行分块
            for(int i=0;i<row;i++){
                int num=0;
                for(int j=0;j<column;j++){
                    if(j==0){
                        while(j+1<column&&puzzle_list[i][j]>=-1){
                            j+=1;
                        }
                    }
                    if(puzzle_list[i][j]>=-1){
                        num+=1;
                        while(j+1<column&&puzzle_list[i][j+1]>=-1){
                            j=j+1;
                        }
                    }
                    if(puzzle_list[i][j]==-2){
                        rowSplit[i][j]=num;
                    }
                }
                numRow[i]=num;
            }
            for(int i=0;i<row;i++){
                for(int j=0;j<numRow[i];j++){
                    rowAva[i][j]=false;
                }
            }
            for(int i=0;i<row;i++){
                for(int j=0;j<column;j++){
                    cout<<rowSplit[i][j]<<" ";
                }
                cout<<endl;
            }
            cout<<endl;
        }
        ~Akari(){
            while(!s1->IsEmpty()){
                Point *p=s1->Pop();
                delete p;
            }
            while(!s2->IsEmpty()){
                Point *p=s2->Pop();
                delete p;
            }
            delete s1;
            delete s2;
        }
        void setStack(){
            for(int i=0;i<row;i++){
                for(int j=0;j<column;j++){
                    if(puzzle_list[i][j]==-2){
                        if(isNextToNum(i,j)){
                            Point *p = new Point;
                            p->x = i;
                            p->y = j;
                            s2->Push(p);
                        }
                    }
                }
            }
            while(!s2->IsEmpty()){
                s1->Push(s2->Pop());
            }
        }
        void showOrigin(){
            if(mode==1){
                cout<<"the original Alari:"<<endl;
                for(int i=0;i<row;i++){
                    for(int j=0;j<column;j++){
                        if(puzzle_list[i][j]>=0) cout<<puzzle_list[i][j]<<" ";
                        else if(puzzle_list[i][j]==-1)
                            cout<<"#"<<" ";
                        else cout<<"[]";
                    }
                    cout<<endl;
                }
            }
        }
        void printResult(){
            cout<<"the result of the Araki:"<<endl;
            if(mode==0){
                for(int i=0;i<row;i++){
                    for(int j=0;j<column;j++){
                        if(puzzle_list[i][j]>=0) cout<<puzzle_list[i][j];
                        else if(puzzle_list[i][j]==-1)
                            cout<<"X";
                        else if(puzzle_list[i][j]==-5)
                            cout<<".";
                        else if(puzzle_list[i][j]==-3)
                            cout<<"*";
                    }
                }
            }
            if(mode==1){
                for(int i=0;i<row;i++){
                    for(int j=0;j<column;j++){
                        if(puzzle_list[i][j]>=0) cout<<puzzle_list[i][j]<<" ";
                        else if(puzzle_list[i][j]==-1)
                            cout<<"X"<<" ";
                        else if(puzzle_list[i][j]==-5)
                            cout<<"."<<" ";
                        else if(puzzle_list[i][j]==-3)
                            cout<<"*"<<" ";
                    }
                    cout<<endl;
                }
            }
        }
        bool isNextToNum(int i,int j){
            if(i-1>=0 && puzzle_list[i-1][j]>=0) return true;
            if(i+1<row && puzzle_list[i+1][j]>=0) return true;
            if(j-1>=0 && puzzle_list[i][j-1]>=0) return true;
            if(j+1<column && puzzle_list[i][j+1]>=0) return true;
            return false;
        }
        bool checkWall1(int i, int j){
            //i,j位置如果是个墙，他周围是否符合灯数要求
            //放灯的时候允许墙周围不够灯
            int count=1;
            if(i-1>=0&&puzzle_list[i-1][j]==-3) count++;
            if(i+1<row&&puzzle_list[i+1][j]==-3) count++;
            if(j-1>=0&&puzzle_list[i][j-1]==-3) count++;
            if(j+1<column&&puzzle_list[i][j+1]==-3) count++;
            if(count>puzzle_list[i][j]) return false;
            return true;
        }
        bool checkWall2(int i, int j){
            //i,j位置如果是个墙，他周围是否符合灯数要求
            //检查的时候用这个
            int count=1;
            if(i-1>=0&&puzzle_list[i-1][j]==-3) count++;
            if(i+1<row&&puzzle_list[i+1][j]==-3) count++;
            if(j-1>=0&&puzzle_list[i][j-1]==-3) count++;
            if(j+1<column&&puzzle_list[i][j+1]==-3) count++;
            if(count!=puzzle_list[i][j]) return false;
            return true;
        }
        bool checkWall(int i, int j){
            //i,j位置如果是个墙，他周围是否符合灯数要求
            //检查的时候用这个
            int count=0;
            if(i-1>=0&&puzzle_list[i-1][j]==-3) count++;
            if(i+1<row&&puzzle_list[i+1][j]==-3) count++;
            if(j-1>=0&&puzzle_list[i][j-1]==-3) count++;
            if(j+1<column&&puzzle_list[i][j+1]==-3) count++;
            if(count!=puzzle_list[i][j]) return false;
            return true;
        }
        bool checkWall3(int x, int y){
            //x,y不是墙，如果放灯会不会导致周围墙的数字不符合要求
            //分上下左右四个方向
            if(x-1>=0&&puzzle_list[x-1][y]>=0){
                //这个表示墙在上方，过了这个方格以后这个墙一定要满足条件，所以用checkWall2，其它情况不用这么严格
                if(checkWall2(x-1,y)==false){
                    return false;
                }
            }
            if(x+1<row&&puzzle_list[x+1][y]>=0){
                if(checkWall1(x+1,y)==false){
                    return false;
                }
            }
            if(y-1>=0&&puzzle_list[x][y-1]>=0){
                if(checkWall1(x,y-1)==false){
                    return false;
                }
            }
            if(y+1<column&&puzzle_list[x][y+1]>=0){
                if(checkWall1(x,y+1)==false){
                    return false;
                }
            }
            return true;
        }

        bool checkRule(int i, int j){
            if(colAva[j][colSplit[i][j]]==false && rowAva[i][rowSplit[i][j]]==false){
                return true;
            }
            else return false;
        }
        bool judgeResult(){
            
            // for(int i=0;i<row;i++){
            //     for(int j=0;j<column;j++){
            //         if(isLighted[i][j]==0){
            //             return false;
            //         }
            //     }
            // }
            // return true;
            for(int i=0;i<row;i++){
                for(int j=0;j<numRow[i];j++){
                    if(rowAva[i][j]==false){
                        return false;
                    }
                }
            }
            for(int i=0;i<column;i++){
                for(int j=0;j<numColumn[i];j++){
                    if(colAva[i][j]==false){
                        return false;
                    }
                }
            }
            return true;
        }
        bool judgeResult2(){
            for(int i=0;i<row;i++){
                for(int j=0;j<column;j++){
                    if(puzzle_list[i][j]>=0){
                        if(!checkWall(i,j)){
                            return false;
                        }
                    }
                }
            }
            return true;
        }
        void helper(int x, int y){
            
            if(x>=row){
                if(judgeResult()){
                    printResult();
                    hasPrinted=true;
                    return;
                }
                return;
            }
            if(judge==0&&s1->IsEmpty()){
                judge=1;
                if(judgeResult2()==false){
                    judge=0;
                    return;
                }
            }
            if(!s1->IsEmpty()){
                judge=0;
                Point *p = s1->Pop();
                s2->Push(p);
                if(checkRule(p->x,p->y)){
                    for(int k=-3;k>-6;k=k-2){
                        puzzle_list[p->x][p->y]=k;
                        if(k==-3){
                            // lightSquare(p->x,p->y);
                            colAva[p->y][colSplit[p->x][p->y]]=true;
                            rowAva[p->x][rowSplit[p->x][p->y]]=true;
                        }
                        helper(x,y);
                        if(hasPrinted){
                            return;
                        }
                        puzzle_list[p->x][p->y]=-2;
                        if(k==-3){
                            colAva[p->y][colSplit[p->x][p->y]]=false;
                            rowAva[p->x][rowSplit[p->x][p->y]]=false;
                        }
                    }
                    s1->Push(s2->Pop());
                }
                else{
                    puzzle_list[p->x][p->y]=-5;
                    helper(x,y);
                    if(hasPrinted){
                        return;
                    }
                    puzzle_list[p->x][p->y]=-2;
                    s1->Push(s2->Pop());
                }
            }
            else{
                if(puzzle_list[x][y]==-2){
                    if(checkRule(x,y)){
                        for(int k=-3;k>-6;k=k-2){
                            //这个是列举-3放灯和-5不放灯两种情况，-2是未定义的方格
                            puzzle_list[x][y]=k;
                            if(k==-3){
                                colAva[y][colSplit[x][y]]=true;
                                rowAva[x][rowSplit[x][y]]=true;
                            }
                            if(y+1>=column)
                                helper(x+1,0);
                                if(hasPrinted){
                                    return;
                                }
                            else
                                helper(x,y+1);
                                if(hasPrinted){
                                    return;
                                }

                            puzzle_list[x][y]=-2;
                            if(k==-3){
                                colAva[y][colSplit[x][y]]=false;
                                rowAva[x][rowSplit[x][y]]=false;
                            }
                        }
                    }
                    else {
                        if(y+1>=column){
                            puzzle_list[x][y]=-5;
                            helper(x+1,0);
                            if(hasPrinted){
                                return;
                            }
                            puzzle_list[x][y]=-2;
                        }
                        else{
                            puzzle_list[x][y]=-5;
                            helper(x,y+1);
                            if(hasPrinted){
                                return;
                            }
                            puzzle_list[x][y]=-2;
                        }
                    }
                }
                else{
                    if(y+1>=column)helper(x+1,0);
                    else helper(x,y+1);
                    if(hasPrinted){
                        return;
                    }
                }
            }
        }
        void solve(){
            setStack();
            init();
            helper(0,0);
        }
};

int main(){
    cout<<"choose Output Mode:"<<endl;
    cout<<"0-Machine format   1-Table format"<<endl;
    int Mode;
    string sd;
    cin>>Mode;
    //cout<<"Please input the number of rows and columns:"<<endl;
    int r,c;
    // cin>>r>>c;
    // cout<<"Please input a Sudoku string:"<<endl;
    // cin>>sd;
    // r=8;
    // c=8;
    // sd="...1X....X....1...1..X..X......0X......1..2..X...1....2....1X...";
    //sd="...1X...X....1..1..X.X......X........2..X..1....2";
    // r=3;
    // c=3;
    // sd = "2....X.1.";
    // sd="2....X.1.";1........1...X.......X...2..X........1.....4............2.....2........X..2...X.......0...1........1
    // r=14;c=24;
    // sd=".........................X.1.X....0..X....X.X.1...X..X.0X.X..X.21.X..X...0.X.X....0..X....X.X.X..........................1XX....1....X....2.XXX........X....2....3............X....1....X........010.X....X....1....XXX..........................X.X.0....X..X....X.1.X...2..X.11.1..1.2X.X..X...1.X.0....X..X....X.X.1.........................";
    r=10;
    c=10;
    sd="1........1...X.......X...2..X........1.....4............2.....2........X..2...X.......0...1........1";
    // r=12;
    // c=12;
    // sd=".0...............X....3.....1..X.......2....X.....1....X.1.......2....2..X....2.......2.X....2.....2....2.......2..3.....1....X...............1.";
    int **sudoku=new int*[r];
    for(int i=0;i<r;i++){
        sudoku[i]=new int[c];
    }
    for(int i=0;i<r;i++){
        for(int j=0;j<c;j++){
            if(sd[i*c+j]=='.'){
                sudoku[i][j]=-2;
            }
            else if(sd[i*c+j]=='X'){
                sudoku[i][j]=-1;
             }
            else{
                sudoku[i][j]=sd[i*c+j]-'0';
            }
        }

    }
    Akari problem = Akari(sudoku, r, c, Mode);
    problem.showOrigin();
    problem.solve();
    return 0;
}
