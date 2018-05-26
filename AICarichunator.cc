#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Carichu


/*void printMatrix(const vector<vector<pair<int,int>>> &board) {
  cout << "printMatrix:" << endl;
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[0].size(); j++) {
      if (board[i][j].first != -1)
        cout << board[i][j].first << "-" << board[i][j].second << " ";
    }
    cout << endl;
  }
  cout << "------------------------------" << endl;

}
*/

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  static constexpr int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
  static constexpr int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

  //////////////////////////////////////////////////
  //
  //
  //////////////////////////////////////////////////

    struct Pos {
        int i;
        int j;
        int val = -2;              /* -2 si no hi ha res
                                      HIGH_VALUE si està dins el post
                                      LOW_VALUE si està al voltant del post
                                      */
        bool visited = false;
    };

    vector<vector<Pos>> init_Board(int id) {
         Data in = data(id);
         vector<vector<Pos>> mat (MAX, vector<Pos>(MAX));
         for (int i = 0; i < MAX; i++) {
             for (int j = 0; j < MAX; j++) {
                mat[i][j].val = post_value(i,j);
                mat[i][j].i = i;
                mat[i][j].j = j;
                mat[i][j].visited = false;
             }
         }
         return mat;
    }


    vector<vector<pair<int, int>>> init_BoardPair() {
        //Matriz para saber de qué qué posición vengo
         pair <int, int> pos;
         vector<vector<pair<int,int>>> mat(MAX, vector<pair<int,int>>(MAX));
         for (int i = 0; i < MAX; i++) {
             for (int j = 0; j < MAX; j++) {
                pos = make_pair (-1,-1);
                mat[i][j] = pos;
             }
         }
         return mat;
    }

    void play_soldier(int id) {
        cout << endl << "Jugando al soldado con id " << id << endl;
        Data in = data(id);
        int i = in.pos.i;
        int j = in.pos.j;
        int player = in.player;

        for (int k = 0; k < 8; k++) {
                int ii = i + I[k];
                int jj = j + J[k];
                if(ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
                  //Ataca si hay un enemigo adyacente
                    int id2 = which_soldier(ii,jj);
                    if(id2 != 0 and data(id2).player != player) {
                        cout << "JAJA loko un soldaooonjajaja piumpiuamamma rataatatat" << endl;
                        command_soldier(id, ii, jj);
                        return;
                    }
                }
        }
        simpleBFS(id, i, j);
    }

        //////////////////////////////////
        //Now it's time for an easy BFS... que no va. HJBGDSANGSADNKDSANADKNSANLKMÑLMVMKD

  void simpleBFS(int id, int i, int j){
        cout << "SimpleBFS del soldat " << id << endl;

        queue<Pos> cua;
        vector<vector<Pos>> board = init_Board(id);
        vector<vector<pair<int,int>>> boardP = init_BoardPair();

        Pos act;
        act.i = i;
        act.j = j;
        board[i][j].visited = true;
        boardP[i][j].first = i;
        boardP[i][j].second = j;

        cua.push(act);
        Pos p;
        cout << endl;
        cout << "soy el jugador " << act.i << " " << act.j << endl;
        cout << endl;
        while(!cua.empty()) {
          p = cua.front();
          cua.pop();

          int val = post_value(p.i, p.j);
          if (val == HIGH_VALUE) {
              cout << "HULA UN POST K WAPO AJAJAJA" << endl;
              amoberse(boardP, p, act, id);
              return;
          }
          else if (val == LOW_VALUE) {
              cout << "gola " << endl;
              amoberse(boardP, p, act, id);
              return;
          }
          else {
              cout << "nada" << endl;
          }


          for (int i = 0; i < 8; i++) {
              Pos t;
              t.i = p.i + I[i];
              t.j = p.j + J[i];
              if (t.j < MAX and t.i < MAX and t.j >= 0 and t.i >= 0 and
                  what(t.i, t.j) != -1 and !board[t.i][t.j].visited and what(t.i, t.j) != MOUNTAIN
                  and what(t.i, t.j) != WATER and fire_time(t.i, t.j) == 0) {
                  board[t.i][t.j].visited = true;
                  boardP[t.i][t.j].first = p.i;
                  boardP[t.i][t.j].second = p.j;
                  cua.push(t);
              }
          }
        }

  }

  //Esta funcion va bien, al menos.
  void amoberse(const vector<vector<pair<int,int>>> &board, Pos fin, Pos ini, int id){
      pair<int, int> inip = make_pair(ini.i, ini.j);
      pair<int, int> finp = make_pair(fin.i, fin.j);
      pair<int, int> ant = finp;

      while(inip != finp){
        ant = finp;
        finp = board[ant.first][ant.second];
      }

      cout << "hola que tal, soy el soldado con id " << id << endl;
      cout << "mi posicion es la " << inip.first << " " << inip.second << endl;
      cout << "soy retarded y me intento mover a " << ant.first << " " << ant.second << endl;

      command_soldier(id, ant.first, ant.second);
  }



  virtual void play () {
      int player = me();
      vector<int> S = soldiers(player);
      for (int i = 0; i < (int)S.size(); i++) play_soldier(S[i]);
  }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
