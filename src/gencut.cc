#include <iostream>
#include <fstream>
#include <gzstream.h>
#include <map>
#include <set>
#include <chrono>
#include <random>
#include <R.h>
#include <Rdefines.h>

using namespace std;

inline map<string,string> get_seqs (string file)
{
    igzstream f (file.c_str());
    string id, seq="";
    map<string,string> mps;
    string s;

    while (getline (f,s)){
        if (s[0] == '>'){
            if (seq != "")
                mps[id] = seq;

            id = s.substr (1,s.find(" ")-1);            
            seq="";
        }
        else
            seq += s;
    }

    mps[id] = seq;

    f.close(); 

    return mps;
}

inline bool chkseq (set<unsigned> &sn, unsigned &n, unsigned &tam)
{
    for (auto t: sn){
        if (n == t)
            return true;
        if ( ((n+tam > t) && (n < t)) || ((n < t+tam) && (n > t)) )
            return true;
    }
    return false;
}

string genocut (string arq, unsigned tam, unsigned quant)
{
    map<string,string> mpseq = get_seqs (arq);

    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 g1 (seed);

    unsigned i, n, j = 0;
    string s, id;
    map<string,string> mpsq;
    set<unsigned> snn;
    const unsigned NUM_INTER = 10;

    for (auto a: mpseq){
        int par = a.second.size() - tam;
        id = a.first; id.erase(0,1);

        if (par < 0){
            mpsq [id+"_"+to_string(i+1)] = a.second;
        }
        else{
            uniform_int_distribution<int> unif (0,par);

            for (i = 0; i < quant; ++i){
                n=unif(g1);                    
                while (n > (a.second.size()-tam-1))
                    n = unif(g1);
                while (chkseq(snn, n, tam) && (j < NUM_INTER)){
                    n = unif(g1);
                    ++j;
                }

                if (j != NUM_INTER)
                    mpsq [id+"_"+to_string(i+1)] = a.second.substr (n,tam);
                
                snn.insert (n);
                j = 0;
            }
        }
    }

    string stab = "";

    for (auto p: mpsq)
        stab += '>' + p.first + '\n' + p.second + '\n';
    stab.pop_back();

    return stab; 
}

extern "C" 
{
    SEXP getseqs (SEXP rfile, SEXP rtam, SEXP rquant)
    {
        string arq = CHAR(STRING_ELT(rfile,0));
        int tam = asInteger (rtam);
        int quant = asInteger (rquant);

        string str = genocut (arq, tam, quant);

        SEXP cstr = allocVector(STRSXP, str.size());
        PROTECT (cstr);
        cstr = mkString(str.c_str());
  		UNPROTECT(1);

        return cstr;
    }
}