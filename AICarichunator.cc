#include "Player.hh"
#include <queue>

/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Carichukiller

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
        int soldados = 0;
    };

    vector<vector<Pos>> init_Board() {
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

    vector<vector<bool>> init_BoardBool() {
        // Matriz para saber las posiciones visitadas
         vector<vector<bool>> mat(MAX, vector<bool>(MAX));
         for (int i = 0; i < MAX; i++) {
             for (int j = 0; j < MAX; j++) {
                mat[i][j] = false;
             }
         }
         return mat;
    }

    void play_soldier(vector<vector<Pos>> &board, int id) {
        //cout << endl << "Jugando al soldado con id " << id << endl;
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
        simpleBFS(board, id, i, j);
    }

        //Now it's time for an easy BFS.

  void simpleBFS(vector<vector<Pos>> &board, int id, int i, int j){
        //cout << endl << "SimpleBFS del soldat " << id << endl;
        Data in = data(id);
        queue<Pos> cua;

        vector<vector<bool>> boardB = init_BoardBool();
        vector<vector<pair<int,int>>> boardP = init_BoardPair();

        Pos act;
        act.i = i;
        act.j = j;
        boardB[i][j] = true;
        boardP[i][j].first = i;
        boardP[i][j].second = j;

        cua.push(act);
        Pos p;
        while(!cua.empty()) {
          p = cua.front();
          cua.pop();

          int val = post_value(p.i, p.j);
          if ((val == HIGH_VALUE or val == LOW_VALUE) and post_owner(p.i, p.j) != in.player) {
              p.soldados++;
              board[p.i][p.j].soldados++;
              //cout << "En la posicion: " << p.i << " " << p.j << " hay un post no conquistado. Me dirijo allí. " << endl;
              //cout << "Ya se dirigen allí estos soldados: " << board[p.i][p.j].soldados << " vs " << p.soldados << "." << endl;
              amoberse(boardP, p, act, id);
              return;
          }

          for (int i = 0; i < 8; i++) {
              Pos t;
              t.i = p.i + I[i];
              t.j = p.j + J[i];
              if (t.j < MAX and t.i < MAX and t.j >= 0 and t.i >= 0 and
                  what(t.i, t.j) != -1 and !boardB[t.i][t.j] and what(t.i, t.j) != MOUNTAIN
                  and what(t.i, t.j) != WATER and fire_time(t.i, t.j) == 0) {
                  boardB[t.i][t.j] = true;
                  boardP[t.i][t.j].first = p.i;
                  boardP[t.i][t.j].second = p.j;
                  cua.push(t);
              }
          }
        }

  }

  void amoberse(const vector<vector<pair<int,int>>> &board, Pos fin, Pos ini, int id){
      pair<int, int> inip = make_pair(ini.i, ini.j);
      pair<int, int> finp = make_pair(fin.i, fin.j);
      pair<int, int> ant = finp;

      while(inip != finp){
        ant = finp;
        finp = board[ant.first][ant.second];
      }
      command_soldier(id, ant.first, ant.second);
  }


  void play_helicopter(int id) {
    Data in = data(id);
    // Si se puede y hay al menos 4 jugadores enemigos se tira napalm.
    if (data(id).napalm == 0) {
      int numsoldados = 0;
      for (int i = 0; i < 8; ++i) {
        int ii = in.pos.i + I[i];
        int jj = in.pos.j + J[i];
        int id2 = which_soldier(ii, jj);
        if (id2 != 0 and data(id2).player != in.player) ++numsoldados;
        if (id2 != 0 and data(id2).player != in.player and numsoldados >= 2) {
            command_helicopter(id, NAPALM);
        }
      }
    }

    vector<pair<int,int>> reloj = {{3,-2},{-2,3},{-3,-2},{-2,-3}};
    int ori = in.orientation;

    // Las posiciones ii y jj corresponden a las posiciones iniciales de lo que
    // hay que analizar.
    int ii = in.pos.i + reloj[ori].first;
    int jj = in.pos.j + reloj[ori].second;
    // snorew indica si es cierto que el helicotero está mirando a este u oeste (hay que sumarle a las is)
    // y si es falso que está mirandoa norte o sur (hay que sumarle a las js).
    bool obstacle = false;
    int t = 0;

    if ((ori == 1 or ori == 3) and not obstacle) {
      // Para analizar hay que incrementar las js.
      while (not obstacle and t < 5){
        if (what(ii+t, jj) == MOUNTAIN or what(ii+t, jj) == -1) obstacle = true;
        else if (ori == 1 and which_helicopter(in.pos.i+t, in.pos.j+5) != 0) obstacle = true;
        else if (ori == 3 and which_helicopter(in.pos.i+t, in.pos.j-5) != 0) obstacle = true;
        t++;
      }
    } else if ((ori == 0 or ori == 2) and not obstacle) {
      // Norte u oeste. Para analizar hay que incrementar las is.
      while (not obstacle and t < 5){
        if (what(ii, jj+t) == MOUNTAIN or what(ii, jj+t) == -1) obstacle = true;
        else if (ori == 0 and which_helicopter(in.pos.i+5, in.pos.j+t) != 0) obstacle = true;
        else if (ori == 2 and which_helicopter(in.pos.i-5, in.pos.j+t) != 0) obstacle = true;
        t++;
      }
    }
    // 50/50 oportunidad de si obstacle gire clockwise/COUNTER_CLOCKWISE
    int g = random(1, 2);
    int c = random(1, 2);
    if (obstacle) command_helicopter(id, g == 1 ? COUNTER_CLOCKWISE : CLOCKWISE);
    else command_helicopter(id, c == 1 ? FORWARD2 : FORWARD1);
  }

  void throw_parachuter(int helicopter_id) {
      // We get the data of the helicopter...
    Data in = data(helicopter_id);
      // ... and try to throw a parachuter, examining the land.
    int ii = in.pos.i + random(-2, 2);
    int jj = in.pos.j + random(-2, 2);
    if (ii >= 0 and ii < MAX and jj >= 0 and jj < MAX
        and what(ii, jj) != MOUNTAIN
        and what(ii, jj) != WATER and fire_time(ii, jj) == 0
        and which_soldier(ii,jj) == 0)
      command_parachuter(ii, jj);
  }

  virtual void play () {
    int player = me();
    if (round() == 199) cout << endl << endl << "Es la última ronda, con SCORE " << endl << total_score(player) << " y CPU " << status(player) << endl;
    vector<int> H = helicopters(player); // helicopters of my player
    vector<int> S =    soldiers(player); //    soldiers of my player

    // If in a random helicopter I have parachuters, I throw one.
    int max = 0;
    while (max < MAX_JUMP){
      int helicopter_id = H[random(0, H.size()-1)];
      if (not data(helicopter_id).parachuters.empty()) throw_parachuter(helicopter_id);
      ++max;
    }

    vector<vector<Pos>> board = init_Board();
    for (int i = 0; i < (int)H.size(); ++i) play_helicopter(H[i]);
    for (int i = 0; i < (int)S.size(); ++i) play_soldier(board, S[i]);
  }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
