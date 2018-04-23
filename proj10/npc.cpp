

using namespace std;

class npc: public character{
  public:
    bool right;
    npc()
    {
      type = NPC;
      right = rand() % 2;
      alive = true;
    };
    ~npc(){ };
};
