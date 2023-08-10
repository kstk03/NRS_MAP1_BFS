/*MAP1_player��item���ŒZ�����Ŏ擾����*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/* MAP��HEIGHT��WIDTH */
#define MAP_HEIGHT 7
#define MAP_WIDTH 11

#define PATH 0
#define WALL 1
#define PASSED 'X'
#define ITEM 'I'
#define START 'S'
#define GOAL 'G'
#define PLAYER 'P'
#define ENEMY_A 'A'
#define ENEMY_B 'B'


typedef enum {//player�̗񋓌^
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef enum {//enemyA�̗񋓌^
    A_DOWN,
    A_LEFT,
    A_UP,
    A_RIGHT
} Direction_enemyA;

typedef enum {//enemyB�̗񋓌^
    B_UP,
    B_LEFT,
    B_DOWN,
    B_RIGHT
} Direction_enemyB;


typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position position;
    int distance;
} QueueNode;

bool isvalidMove(Position position);
bool isGoal(Position position);
int calculateDistance(Position start, Position end);
void collectItems();
void reachGoal();
void moveEnemyA();
void moveEnemyB();
void printMap(void);

Position player, start, enemyA, enemyB, goal;
bool visited[MAP_HEIGHT][MAP_WIDTH];

const int items = 5; //�A�C�e���̑���
int remainingItems;      // ITEM�̎c��̐�
int score;
int turns;



/* MAP�̏ڍ� */
static int MAP[MAP_HEIGHT][MAP_WIDTH] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 'I', 0, 0, 'A', 1, 'B', 0, 0, 'G', 1},
    {1, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1},
    {1, 0, 1, 0, 'I', 1, 'I', 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 'S', 0, 0, 0, 'I', 0, 0, 0, 'I', 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

/* MAP�̕\�� */
void printMap(void)
{
    int x, y;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            /* �z��̒l�ɉ����ċL��/������\�� */
            if (MAP[y][x] == PATH)
            {
                printf(" ");
            }
            if (MAP[y][x] == WALL)
            {
                printf("#");
            }
            if (MAP[y][x] == ITEM)
            {
                printf("��");
            }
            if (MAP[y][x] == START)
            {
                printf("S");
            }
            if (MAP[y][x] == GOAL)
            {
                printf("G");
            }
            if (MAP[y][x] == ENEMY_A)
            {
                printf("A");
            }
            if (MAP[y][x] == ENEMY_B)
            {
                printf("B");
            }
            if (MAP[y][x] == PLAYER)
            {
                printf("P");
            }
        }
        printf("\n");
    }
    printf("\n");
}

bool isvalidMove(Position position) {

    if (position.x >= 0 && position.x < MAP_WIDTH && position.y >= 0 && position.y < MAP_HEIGHT && MAP[position.y][position.x] == PATH)
    {
        return true;
    }
    return false;
}


/* GOAL�̊m�F */
bool isGoal(Position position)
{
    if (turns > 0 && remainingItems == 0 && position.x == goal.x && position.y == goal.y)
    {
        return true;
    }
    return false;
}

// ���D��T�����g�p���čŒZ�������v�Z
int calculateDistance(Position start, Position end) {
    // ������
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            visited[y][x] = false;
        }
    }

    QueueNode queue[MAP_HEIGHT * MAP_WIDTH];
    int front = 0, rear = 0;

    // �����ʒu���L���[�ɒǉ�
    queue[rear++] = { start, 0 };
    visited[start.y][start.x] = true;

    while (front < rear) {
        QueueNode currentNode = queue[front++];
        Position currentPos = currentNode.position;
        int currentDistance = currentNode.distance;

        if (currentPos.x == end.x && currentPos.y == end.y) {
            return currentDistance;
        }

        // �㉺���E�̗אڃZ���𒲂ׂ�
        Position moves[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
        for (int i = 0; i < 4; i++) {
            Position newPos = { currentPos.x + moves[i].x, currentPos.y + moves[i].y };

            if (isvalidMove(newPos) && !visited[newPos.y][newPos.x]) {
                queue[rear++] = { newPos, currentDistance + 1 };
                visited[newPos.y][newPos.x] = true;
            }
        }
    }

    return -1; // ���B�s�\
}
//item���W�߂�֐�
void collectItems() {
    Position itemsPosition[items];
    int itemDistances[items];

    int itemCount = 0;
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (MAP[y][x] == ITEM) {
                itemsPosition[itemCount++] = { x, y };
            }
        }
    }

    for (int i = 0; i < itemCount; i++) {
        itemDistances[i] = calculateDistance(player, itemsPosition[i]);
    }

    // �ł��߂��A�C�e����I��
    int minDistance = MAP_WIDTH + MAP_HEIGHT;  // �\���ɑ傫�ȏ����l
    int closestItemIndex = -1;

    for (int i = 0; i < itemCount; i++) {
        if (itemDistances[i] >= 0 && itemDistances[i] < minDistance) {
            minDistance = itemDistances[i];
            closestItemIndex = i;
        }
    }

    if (closestItemIndex >= 0) {
        Position closestItemPosition = itemsPosition[closestItemIndex];
        int dx = closestItemPosition.x - player.x;
        int dy = closestItemPosition.y - player.y;

        Position newPlayerPosition = player;

        if (abs(dx) > abs(dy)) {
            newPlayerPosition.x += (dx > 0) ? 1 : -1;
        }
        else {
            newPlayerPosition.y += (dy > 0) ? 1 : -1;
        }

        if (isvalidMove(newPlayerPosition) || MAP[newPlayerPosition.y][newPlayerPosition.x] == ITEM) {
            if (MAP[newPlayerPosition.y][newPlayerPosition.x] == ITEM) {
                remainingItems--;
                printf("�c���ITEM�̐���%d�ł�\n", remainingItems);
            }
            MAP[player.y][player.x] = PATH;
            MAP[newPlayerPosition.y][newPlayerPosition.x] = PLAYER;
            player = newPlayerPosition;
        }
    }
}

// Goal�ɍs���֐�
void reachGoal() {
    Position newPlayerPosition = player;

    // �S�[���ւ̍ŒZ�������v�Z
    int minDistance = calculateDistance(player, goal);

    // �㉺���E�̈ړ��������čŒZ�����̈ʒu��T��
    Position moves[4] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
    for (int i = 0; i < 4; i++) {
        Position newPos = { player.x + moves[i].x, player.y + moves[i].y };

        if (isvalidMove(newPos)) {
            int distance = calculateDistance(newPos, goal);
            if (distance < minDistance) {
                minDistance = distance;
                newPlayerPosition = newPos;
            }
        }
    }

    // �V�����ʒu���L���ł���΃v���C���[���ړ�������
    if (newPlayerPosition.x != player.x || newPlayerPosition.y != player.y) {
        MAP[player.y][player.x] = PATH;
        MAP[newPlayerPosition.y][newPlayerPosition.x] = PLAYER;
        player = newPlayerPosition;
    }
}

// EnemyA�̈ړ��֐�
void moveEnemyA() {
    int dx = player.x - enemyA.x;
    int dy = player.y - enemyA.y;

    Position New = enemyA;
    bool moved = false; // �ړ��������������̃t���O

    //1�ڂ̏���
    if (dy != 0) {
        New.y = enemyA.y + (dy > 0 ? 1 : -1);
        if (isvalidMove({ enemyA.x, New.y }) && MAP[New.y][enemyA.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[New.y][enemyA.x] = 'A';
            enemyA.y = New.y;
            moved = true;
            return;
        }
    }
    //2�ڂ̏���
    if (dx != 0) {
        New.x = enemyA.x + (dx > 0 ? 1 : -1);
        if (isvalidMove({ New.x, enemyA.y }) && MAP[enemyA.y][New.x] != ITEM) {
            MAP[enemyA.y][enemyA.x] = PATH;
            MAP[enemyA.y][New.x] = 'A';
            enemyA.x = New.x;
            moved = true;
            return;
        }
    }
    //3�ڂ̏���
    if (!moved) {
        Direction_enemyA nextDirections[4] = { A_DOWN, A_LEFT, A_UP, A_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyA;
            Direction_enemyA nextDirection = nextDirections[i];

            switch (nextDirection) {
            case A_UP:
                New.y--;
                break;
            case A_RIGHT:
                New.x++;
                break;
            case A_DOWN:
                New.y++;
                break;
            case A_LEFT:
                New.x--;
                break;
            }
            if (isvalidMove(New)) {
                MAP[enemyA.y][enemyA.x] = PATH;
                MAP[New.y][New.x] = ENEMY_A;
                enemyA = New;
                break;
            }
        }
    }
}

// EnemyB�̈ړ��֐�
void moveEnemyB() {
    int dx = player.x - enemyB.x;
    int dy = player.y - enemyB.y;

    Position New = enemyB;
    bool moved = false; // �ړ��������������̃t���O

    //����1
    if (dx != 0) {
        New.x = enemyB.x + (dx > 0 ? 1 : -1);/*�������Z�q��p����y���� ? �^�̏ꍇ�̒l : �U�̏ꍇ�̒l�z(dx > 0 : ++1) or (dx < 0 : --1)*/
        if (isvalidMove({ New.x, enemyB.y }) && MAP[enemyB.y][New.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[enemyB.y][New.x] = 'B';
            enemyB.x = New.x;
            moved = true;
            return;
        }
    }

    //����2

    if (dy != 0) {
        New.y = enemyB.y + (dy > 0 ? 1 : -1);
        if (isvalidMove({ enemyB.x, New.y }) && MAP[New.y][enemyB.x] != ITEM) {
            MAP[enemyB.y][enemyB.x] = PATH;
            MAP[New.y][enemyB.x] = 'B';
            enemyB.y = New.y;
            moved = true;
            return;
        }
    }

    //����3
    if (!moved) {
        Direction_enemyB nextDirections[4] = { B_UP ,B_LEFT, B_DOWN, B_RIGHT };

        for (int i = 0; i < 4; i++) {
            New = enemyB;
            Direction_enemyB nextDirection = nextDirections[i];

            switch (nextDirection) {
            case B_UP:
                New.y--;
                break;
            case B_RIGHT:
                New.x++;
                break;
            case B_DOWN:
                New.y++;
                break;
            case B_LEFT:
                New.x--;
                break;
            }
            if (isvalidMove(New)) {
                MAP[enemyB.y][enemyB.x] = PATH;
                MAP[New.y][New.x] = ENEMY_B;
                enemyB = New;
                break;
            }
        }
    }
}

int main(void) {
    //player�̏����l
    player.x = 1;
    player.y = 5;
    //enemyA�̏����l
    enemyA.x = 4;
    enemyA.y = 1;
    //enemyB�̏����l
    enemyB.x = 6;
    enemyB.y = 1;

    //goal�̈ʒu
    goal.x = 9;  
    goal.y = 1;

    remainingItems = 5;  // �c���ITEM�̐�
    turns = 50;         // �^�[����

    printf("MAP1�̏ڍ�\n");
    printf("--------------------\n");
    printf("#:��, ��:�A�C�e��, S:START, G:GOAL\n\n");
    printMap();
    printf("--------------------\n");
    while (turns > 0) {
        if (remainingItems == 0 && player.x == goal.x && player.y == goal.y) {
            printf("�S�[�����܂���!\n");
            printf("�X�R�A��%d�ł�!", score);
            break;
        }

        if (remainingItems > 0) {
            collectItems();
        }
        else if (remainingItems == 0) {
            reachGoal();
        }


        moveEnemyA();
        moveEnemyB();
        printMap();//player�Eenemy���ړ��������MAP�̏��
        score = turns; // �^�[����-1�����O�̒l���X�R�A�ƂȂ�
        turns--;
        printf("�^�[��: %d\n", turns);

        if (turns == 0) {
            printf("�^�[�����Ȃ��Ȃ�܂����B�S�[���ł��܂���ł���\n");
        }
    }
    printMap();

    return 0;
}
