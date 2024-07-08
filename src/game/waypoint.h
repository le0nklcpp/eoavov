namespace ai
{
    struct avoidset;
    struct waypoint;
    const int MAXWAYPOINTS = USHRT_MAX - 2;
    const int MAXWAYPOINTLINKS = 6;
    const int WAYPOINTRADIUS = 16;
    const int NUMPREVNODES = 6;

    const float MINWPDIST       = 4.f;     // is on top of
    const float CLOSEDIST       = 32.f;    // is close
    const float FARDIST         = 128.f;   // too far to remap close
    const float JUMPMIN         = 4.f;     // decides to jump
    const float JUMPMAX         = 32.f;    // max jump
    const float SIGHTMIN        = 64.f;    // minimum line of sight
    const float SIGHTMAX        = 1024.f;  // maximum line of sight
    const float VIEWMIN         = 90.f;    // minimum field of view
    const float VIEWMAX         = 180.f;   // maximum field of view
    extern bool route(creatureEntity *d, int node, int goal, vector<int> &route, const avoidset &obstacles, int retries = 0);
    extern void navigate();
    extern void clearwaypoints(bool full = false);
    extern void seedwaypoints();
    extern void loadwaypoints(bool force = false, const char *mname = NULL);
    extern void savewaypoints(bool force = false, const char *mname = NULL);
    extern int showwaypoints, dropwaypoints;
    extern int closestwaypoint(const vec &pos, float mindist, bool links, creatureEntity *d = NULL);
    extern void findwaypointswithin(const vec &pos, float mindist, float maxdist, vector<int> &results);
    extern void inferwaypoints(creatureEntity *d, const vec &o, const vec &v, float mindist = 32.);
    extern vector<waypoint>waypoints;
    struct waypoint
    {
        vec o;
        float curscore, estscore;
        int weight;
        ushort route, prev;
        ushort links[MAXWAYPOINTLINKS];

        waypoint() {}
        waypoint(const vec &o, int weight = 0) : o(o), weight(weight), route(0) { memset(links, 0, sizeof(links)); }

        int score() const { return int(curscore) + int(estscore); }

        int find(int wp)
        {
            loopi(MAXWAYPOINTLINKS) if(links[i] == wp) return i;
            return -1;
        }
        bool haslinks() { return links[0]!=0; }
    };
    static inline bool iswaypoint(int n)
    {
        return n > 0 && n < waypoints.length();
    }

    struct avoidset
    {
        struct obstacle
        {
            void *owner;
            int numwaypoints;
            float above;

            obstacle(void *owner, float above = -1) : owner(owner), numwaypoints(0), above(above) {}
        };

        vector<obstacle> obstacles;
        vector<int> waypoints;

        void clear()
        {
            obstacles.setsize(0);
            waypoints.setsize(0);
        }

        void add(void *owner, float above)
        {
            obstacles.add(obstacle(owner, above));
        }

        void add(void *owner, float above, int wp)
        {
            if(obstacles.empty() || owner != obstacles.last().owner) add(owner, above);
            obstacles.last().numwaypoints++;
            waypoints.add(wp);
        }

        void add(avoidset &avoid)
        {
            waypoints.put(avoid.waypoints.getbuf(), avoid.waypoints.length());
            loopv(avoid.obstacles)
            {
                obstacle &o = avoid.obstacles[i];
                if(obstacles.empty() || o.owner != obstacles.last().owner) add(o.owner, o.above);
                obstacles.last().numwaypoints += o.numwaypoints;
            }
        }

        void avoidnear(void *owner, float above, const vec &pos, float limit);

        #define loopavoid(v, d, body) \
            if(!(v).obstacles.empty()) \
            { \
                int cur = 0; \
                loopv((v).obstacles) \
                { \
                    const ai::avoidset::obstacle &ob = (v).obstacles[i]; \
                    int next = cur + ob.numwaypoints; \
                    if(ob.owner != d) \
                    { \
                        for(; cur < next; cur++) \
                        { \
                            int wp = (v).waypoints[cur]; \
                            body; \
                        } \
                    } \
                    cur = next; \
                } \
            }

        bool find(int n, creatureEntity *d) const
        {
            loopavoid(*this, d, { if(wp == n) return true; });
            return false;
        }

        int remap(creatureEntity *d, int n, vec &pos, bool retry = false);
    };
};