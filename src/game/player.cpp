#include "eoavov.h"
VARFP(min_carry_dist,0,7,1000,fixcarrydist());
VARFP(max_carry_dist,0,10,1000,fixcarrydist());
FVAR(min_dragging_dist,0,20,1000);
FVAR(max_dragging_dist,0,30,1000);
VAR(LOSE_DELAY,0,3000,32767);
FVAR(THROW_FORCE,0.,212.0,32767.);
VARF(togglewalk,0,0,1,game::recountspeed(togglewalk));
FVARF(PLAYER_DEFAULT_SPEED,0.,200.,5000.,game::recountspeed(togglewalk));
FVARF(PLAYER_WALK_SPEED,0.,100.,5000.,game::recountspeed(togglewalk));
FVAR(MAX_INTERACT_DIST,0.,24.,1000.);
VAR(NEXT_USE_DELAY,0,200,32767);
VAR(lock_movement,0,0,1);
VAR(lock_control,0,0,1);
VARF(flash_enabled,0,1,1,{if(!flash_enabled&&isconnected())player1->light = false;});
GMCMD(flashlight,"i",(int*check),{if(!(*check)&&flash_enabled)player1->light = !player1->light;intret(player1->light);});
VAR(cameratilt,-90,1,90);
VAR(camrot,-90,0,90);
VAR(ENABLE_DOUBLEJUMP,1,0,1);

void fixcarrydist()
{
    max_carry_dist = max(min_carry_dist,max_carry_dist);
}
void playerEnt::reset()
{
    lastnode = -1;
    doublejump = false;
    vmodel[0]='\0';
    fpsEntity::reset();
    RPGObject::reset();
    state = CS_ALIVE;
    maxspeed = PLAYER_DEFAULT_SPEED;
    nextinteracttime = lastdamagetime = lastvanimtime = 0;
    movable = true;
    light = false;
    carries = NULL;
    dragging = NULL;
}
void playerEnt::setvmodel(const char*path,int anim,int animtime)
{
    copystring(vmodel,path);
    vanim = (ANIM_GAMESPECIFIC + max(anim,0)) | ANIM_LOOP;
    lastvanimtime = lastmillis-animtime;
}
void playerEnt::removeitem(int index)
{
    RPGObject::removeitem(index);
    if(hands==NULL)setvmodel("\0",0,0);
}
void playerEnt::dropitem(int index) // well, it could've been worse
{
    invItem*item = inv.getitem(index);
    int i;
    if(!item)return;
    RPGItemEnt * e = (RPGItemEnt*)game::fpsents.add(new RPGItemEnt(item));
    for(i=min_carry_dist;i<max_carry_dist;i++)
    {
        e->setpos(vec(o).add(vec(cameradir()).mul(i)));
       if(!collide(e)&&!collideinside)break;
    }
    if(i==max_carry_dist) // throw it under player feet
    {
        vec oldpos = o;
        e->setpos(feetpos().add(vec(0,0,e->eyeheight+e->aboveeye)));
        setpos(vec(o).add(vec(0,0,e->eyeheight+e->aboveeye+1.f)));
        if((collide(this)||collideinside)||(collide(e)||collideinside)) // collideinside resets
        {
            setpos(oldpos);
            game::fpsremove(e);
            return;
        }
    }
    else e->vel = vec(cameradir()).mul(THROW_FORCE).add(vel);
    e->physstate = PHYS_FALL;
    removeitem(index);
}
inline void playerEnt::moveitem()
{
    if(carries)
    {
        vec oldpos = carries->o;
        int i=max_carry_dist;
        while(i>=min_carry_dist)
        {
            carries->o = vec(o).add(vec(cameradir()).mul(i));
            if((!collide(carries)&&!collideinside))
            {
                carries->resetinterp();
                return;
            }
        i--;
        }
    carries->o = oldpos;
    dropent();
    }
    if(dragging)
    {
        vec ppos = feetpos();
        if(ppos.dist(dragging->o) < min_dragging_dist)return;
        if(ppos.dist(dragging->o) > max_dragging_dist || (o.z - dragging->o.z) > min_dragging_dist)
        {
            stopdragging();
            return;
        }
        bool upped = false;
        if(dragging->onfloor())
        {
            dragging->o.addz(1.0f);
            upped = true;
        }
        dragging->turnto(ppos);
        dragging->pitch = 0;
        vec d = dragging->cameradir();
        if(upped)dragging->o.addz(-1.0f);
        while(ppos.dist2(dragging->o) > min_dragging_dist)
        {
            if(collide(dragging, d, 0.0f, true, true))break;
            dragging->o.add(d);
        }
        dragging->resetinterp();
    }
}
void playerEnt::move()
{
    if(!movable)return;
    if(lock_movement)player1->stopmoving();
    crouchplayer(this, 1, true);
    moveplayer(this, 1, true);
    moveitem();
}
void playerEnt::falldamage(int ftime)
{
    doublejump = false;
}
void playerEnt::stopdragging()
{
    dragging = NULL;
    game::recountspeed(togglewalk);
}
void playerEnt::dropent()
{
    if(carries)
    {
        carries->resetinterp();
        if(carries->type==E_MOVABLE)
        {
        propEnt*cast = (propEnt*)carries;
        cast->dropped();
        }
    }
    carries = NULL;
}
void playerEnt::grabent(fpsEntity*e)
{
    carries = e;
}
void playerEnt::killed(fpsEntity*killer)
{
    dropent();
    state = CS_DEAD;
    roll = 0;
    yaw = pitch = 90;
    nextthink = lastmillis + LOSE_DELAY;
    game::cubeevent("player_killed");
}
void playerEnt::quickswitch()
{
    if(!holster)return;
    invItem*buf = hands;
    hands = holster;
    holster = buf;
}
void playerEnt::think()
{
    if(state!=CS_DEAD)move();
    int tilt = camrot;
    if(k_left)tilt -= cameratilt;
    else if(k_right)tilt += cameratilt;
    setcamtilt(tilt);
}
void playerEnt::attack(bool down)
{
    if(nextattacktime>lastmillis)return;
    if(carries)
    {
        fpsEntity* ent = carries;
        dropent();
        ent->vel.add(vec(cameradir()).mul(THROW_FORCE).div(ent->mass));
        nextattacktime = lastmillis + NEXT_USE_DELAY;
    }
    else if(hands)
    {
        hands->parent->use(this,hands,down);
    }
}
void playerEnt::attack2(bool down)
{
    if(crouching&&!down)
    {
        fpsEntity*ent = game::rayfpsent(o,cameradir(),MAX_INTERACT_DIST,false);
        if(ent&&ent->draggable())
        {
            if(dragging != ent)dragging = ent;
            else stopdragging();
            game::recountspeed(togglewalk);
        }
    }
}
bool playerEnt::setev(int attr,const char*val)
{
    if(!fpsEntity::setev(attr,val)&&val)
    {
        #define setv(a) a = lastmillis + atoi(val)
        switch(attr)
        {
        case(EV_NEXTATTACKTIME):setv(nextattacktime);break;
        case(EV_NEXTINTERACTTIME):setv(nextinteracttime);break;
        default:return false;break;
        }
    }
 return true;
}
bool playerEnt::getev(int attr)
{
    #define getv(a) intret(a)
    if(!fpsEntity::getev(attr))
    {
        switch(attr)
        {
        case(EV_NEXTATTACKTIME):getv(nextattacktime);break;
        case(EV_NEXTINTERACTTIME):getv(nextinteracttime);break;
        default:return false;break;
        }
    }
    return true;
}
invItem* playerEnt::draw(int index)
{
    if(index==-1)
    {
        hands = NULL;
        setvmodel("\0",0,0);
        return NULL;
    }
    invItem*item = RPGObject::draw(index);
    if(item)
    {
        item->parent->draw(this,item);
    }
    return item;
}