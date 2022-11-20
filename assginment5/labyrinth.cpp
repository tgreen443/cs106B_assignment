#include "labyrinth.h"
#include "demo/MazeGenerator.h"
#include "testing/SimpleTest.h"
using namespace std;

/* Change kYourName constant to your full name.
 *
 * WARNING: Once you've set set this constant and started exploring your labyrinth,
 * do NOT edit the value of kYourName. Changing kYourName will change which
 * maze you get back, which might invalidate all your hard work!
 */
const string kYourName = "yangxiaokai";

/* Change this constant to the path out of your labyrinth. */
const string kPathOutOfNormalMaze = "EEESSWNSSWWNSENNW";

/**
 * Given a location in a maze, returns whether the given sequence of
 * steps will let you escape the maze.
 *
 * To escape the maze, you need to collect all items in the `needs` set.
 * You can only take steps in the four cardinal directions,
 * and you can't move in directions that don't exist in the maze.
 */
//是否是合法的移动
bool isContainValidChar(string moves){
    for(int i=0;i<moves.size();i++){
        if(moves[i]!='N'&&moves[i]!='S'&&moves[i]!='E'&&moves[i]!='W'){
            return false;
        }
    }

    return true;
}
//生成合法的移动的集合
Set<MazeCell*> generateValidMoves(MazeCell* currentLoc){
    Set<MazeCell*> ValidMoves;
    if(currentLoc->east!=nullptr){
        ValidMoves.add(currentLoc->east);
    }
    if(currentLoc->west!=nullptr){
        ValidMoves.add(currentLoc->west);
    }
    if(currentLoc->north!=nullptr){
        ValidMoves.add(currentLoc->north);
    }
    if(currentLoc->south!=nullptr){
        ValidMoves.add(currentLoc->south);
    }
    return ValidMoves;
}
//生成下一次的移动的指针
MazeCell* returnNextLoc(MazeCell* currentLoc,int i,string moves){
    char nextChar=moves[i];
    if(nextChar=='N'){
        return currentLoc->north;
    }
    if(nextChar=='S'){
        return currentLoc->south;
    }
    if(nextChar=='W'){
        return currentLoc->west;
    }
        return currentLoc->east;
}
bool isPathToFreedom(MazeCell* start, string moves, Set<string> needs) { 
    //如果输入路径包含无效字符（“N”、“S”、“E” 或 “W” 以外的任何字符），请使用 error() 函数报告错误
    if(!isContainValidChar(moves)){
        error("there are valid moves in the path !");
    }
    MazeCell* currentLoc=start;
    Set<string> gets;
    for(int i=0;i<moves.size();i++){
        //但是，如果路径的任何步骤尝试访问不存在的链接（单元格的指针为 nullptr），函数应该停止并返回 false。例如，在上例中尝试从包含魔杖的单元格向东移动时，就能说明该路径不合法
        MazeCell* nextLoc=returnNextLoc(currentLoc,i,moves);
        if(nextLoc==nullptr){
            return false;
        }
        //收集物品
        if(currentLoc->contents!=""){
           gets.add(currentLoc->contents);
        }
        currentLoc=nextLoc;
    }
    //收集最后一个位置的物品
    if(currentLoc->contents!=""){
       gets.add(currentLoc->contents);
    }
    if(gets>=needs){
        return true;
    }
    return false;
}


/* * * * * * Test Cases Below This Point * * * * * */

PROVIDED_TEST("Check paths in the sample from writeup") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    auto maze = toMaze({"* *-W *",
                        "| |   |",
                        "*-* * *",
                        "  | | |",
                        "S *-*-*",
                        "|   | |",
                        "*-*-* P"});

    /* These paths are the ones in the handout. They all work. */
    EXPECT(isPathToFreedom(maze[2][2], "ESNWWNNEWSSESWWN", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "SWWNSEENWNNEWSSEES", allThree));
    EXPECT(isPathToFreedom(maze[2][2], "WNNEWSSESWWNSEENES", allThree));

    /* This path works to pick up just potion */
    EXPECT(isPathToFreedom(maze[2][2], "ESNW", {"Potion"}));

    /* This path doesn't work, since doesn't pick up all items. */
    EXPECT(!isPathToFreedom(maze[2][2], "ESNW", allThree));

    /* This path doesn't work, cannot walk through walls. */
    EXPECT(!isPathToFreedom(maze[2][2], "NNWWSSSEEE", allThree));
}

PROVIDED_TEST("Works when start on an item") {
    auto maze = toMaze({"P-S-W"});

    EXPECT(isPathToFreedom(maze[0][0], "E", {"Potion"}));
    EXPECT(isPathToFreedom(maze[0][1], "W", {"Potion", "Spellbook"}));
}

PROVIDED_TEST("Reports errors if given illegal characters") {
    auto maze = toMaze({"P-S-W"});

    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Q", {"Wand"}));
    EXPECT_ERROR(isPathToFreedom(maze[0][0], "Ee", {"Wand"}));
}

PROVIDED_TEST("Escape from your personal labyrinth!") {
    Set<string> allThree = {"Spellbook", "Potion", "Wand"};
    /* A maze for you to escape from. This maze will be personalized
     * based on the constant kYourName.
     */
    EXPECT(kYourName != "TODO");
    MazeCell* startLocation = mazeFor(kYourName);

    /* Set a breakpoint here. As a reminder, the labyrinth you
     * get will be personalized to you, so don't start exploring the labyrinth
     * unless you've edited the constant kYourName to include your name(s)!
     * Otherwise, you'll be escaping the wrong labyrinth.
     */
    EXPECT(kPathOutOfNormalMaze != "TODO");
    EXPECT(isPathToFreedom(startLocation, kPathOutOfNormalMaze, allThree));
}

