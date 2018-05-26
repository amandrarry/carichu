#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Carichu
static constexpr int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
static constexpr int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

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
             }
         }
         return mat;
    }

    vector<vector<bool>> init_BoardBool(int id) {
         Data in = data(id);
         vector<vector<bool>> mat (MAX, vector<bool>(MAX));
         for (int i = 0; i < MAX; i++) {
             for (int j = 0; j < MAX; j++) {
                mat[i][j] = false;
             }
         }
         mat[data.pos.i][data.pos.j] = true;
         return mat;
    }

    vector<vector<pair<int,int>> init_BoardPair(int id) {
         Data in = data(id);
         pair <int, int> pos;
         vector<vector<pair<int,int>> mat (MAX, vector<vector<pair<int,int>>(MAX));
         for (int i = 0; i < MAX; i++) {
             for (int j = 0; j < MAX; j++) {
                pos = make_pair (-1,-1);
                mat[i][j] = pos;
             }
         }
         return mat;
    }

    void play_soldier(int id) {
        Data in = data(id);
        int i = in.pos.i;
        int j = in.pos.j;
        int player = in.player;
        for (int k = 0; k < 8; k++) {
                int ii = i + I[k];
                int jj = i + J[k];
                if(ii >= 0 and ii < MAX and jj >= 0 and jj < MAX) {
                    int id2 = which_soldier(ii,jj);
                    if(id2 and data(id2).player != player) {
                        command_soldier(id, ii, jj);
                        return;
                    }
                }
        }

        vector<Pos> cami = simpleBFS();
    }

        //////////////////////////////////
        //Now it's time for an easy BFS...
        //////////////////////////////////
  vector<Pos> simpleBFS(){
        queue<Pos> cua;
        Pos act;
        act.i = i;
        act.j = j;

        vector<vector<bool>> boardB = init_BoardBool(id);
        vector<vector<pair<int,int>> boardP = init_BoardPair(id);

        // pair<int, int> pos;

        act.val = board[i][j].val;
        boardB[i][j] = true;
        cua.push(act);
        Pos p;

        while(!cua.empty()) {
          p = cua.front();
          cua.pop();

          cout << p.i << " " << p.j << "visited: " << board[i][j].visited << endl;

          int val = post_value(p.i, p.j);

          if (val == HIGH_VALUE) {
              amoberse(boardP, p, act, id);
          }
            /*
            else if (val == LOW_VALUE && !closest_low) {
                closest_low = true;
                best_low = p;
                best_low.val = LOW_VALUE;
            }
            */

          for (int i = 0; i < 8; i++) {
              Pos t;
              t.i = p.i + I[i];
              t.j = p.j + J[j];
              if (what(t.i, t.j) != -1 and !boardB[t.i][t.j] and what(t.i, t.j) != MOUNTAIN
                  and what(t.i, t.j) != WATER and fire_time(t.i, t.j) == 0) {
                  boardB[t.i][t.j] = true;
                  boardP[t.i][t.j].first = p.i;
                  boardP[t.i][t.j].first = p.j;
                  cua.push(t);
              }
          }
    }
  }

  void amoberse(vector<vector<pair<int,int>> boardP, Pos fin, Pos ini, id){
      pair<int, int> ini = make_pair(ini.j, ini.i);
      pair<int, int> fin = make_pair(fin.j, fin.i);
      pair<int, int> ant;

      while(ini != fin){
        ant = fin;
        fin = boardP[ant.first][ant.second];
      }

      int i = ini.first - ant.first;
      int j = ini.second - ant.second;
      command_soldier(id, i, j);
  }



  virtual void play () {
      int player = me();
      vector<int> S = soldiers(player);
      for (int i = 0; i < (int)S.size(); i++) play_soldier(S[i]);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
