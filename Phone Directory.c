#include<stdio.h>
#include<stdlib.h>

#define SPACE 10

struct node
{
    char name[50], contact[20];
    struct node *lchild;
    struct node *rchild;
};

void printContact(char x[], char y[])
{
    printf("%c %20s %c %20s %c\n", 186, x, 186, y, 186);
}

void displayEnd()
{
    int i;
    printf("%c", 200);
    for(i=0; i<22; i++)
        printf("%c", 205);
    printf("%c", 202);
    for(int i=0; i<22; i++)
        printf("%c", 205);
    printf("%c", 188);
}

int charcmp(char c1, char c2)
{
    //convert both argument chars to capital chars.
    int r;

    c1 = (c1 >='a' && c1<='z')? c1-32 : c1;

    c2 = (c2 >='a' && c2<='z')? c2-32 : c2;

    if(c1 == c2)
        r = 0;
    else if(c1 > c2)
        r = 1;
    else
        r = -1;

    return r;
}
int strcasecmp(char* s1, char* s2)
{
    int a, b, i, l, result;
    char s[20];

    a = strlen(s1);
    b = strlen(s2);
    if(a <= b)
        l = a;
    else
        l = b;

    for(i=0; i<l; i++)
    {
        result = charcmp(s1[i], s2[i]);   //c1 = s1[i]  and  c2 = s2[i]
        if(result > 0)
            return result;
        else if(result < 0)
            return result;
    }
    if(a > b)
        return 1;
    else if(a < b)
        return -1;
    else
        return 0;
}

int height(struct node* ptr)
{
    if (ptr == NULL)
        return -1;
    else
    {
        int lheight = height(ptr -> lchild);
        int rheight = height(ptr -> rchild);

        if (lheight > rheight)
            return (lheight + 1);
        else
            return (rheight + 1);
    }
}

int getBalanceFactor(struct node* ptr)
{
    if (ptr == NULL)
        return -1;
    return height(ptr -> lchild) - height(ptr -> rchild);
}

struct node* rightRotate(struct node* x)
{
    struct node *y;
    y = x->lchild;
    x->lchild = y->rchild;
    y->rchild = x;
    return y;
}

struct node* leftRotate(struct node* x)
{
    struct node *y ;
    y = x->rchild;
    x->rchild = y->lchild;
    y->lchild = x;
    return y;
}

void inorder(struct node* head, FILE* fp)
{
    if(head != NULL)
    {
        inorder(head->lchild, fp);

        fprintf(fp, "%s %s\n", head->name, head->contact);
        fflush(fp);

        printContact(head->name, head->contact);

        inorder(head->rchild, fp);
    }
}

struct node* insert(char* name, char* contact, struct node* ptr)
{
    int cmp;
    if(ptr==NULL)
    {
        ptr = (struct node*)malloc(sizeof(struct node));
        ptr->lchild = NULL;
        ptr->rchild = NULL;
        strcpy(ptr->name, name);
        strcpy(ptr->contact, contact);
    }
    else
    {
        cmp = strcasecmp(name, ptr->name);
        if(cmp<0)
        {
            ptr->lchild = insert(name, contact, ptr->lchild);
        }
        else if(cmp>0)
        {
            ptr->rchild = insert(name, contact, ptr->rchild);
        }
        else
        {
            printf("ELEMENT REPEATED!!!\n");
            return ptr;
        }

        int bf = getBalanceFactor(ptr);
        // Left Left Case
        if(bf > 1 && strcasecmp(name, ptr->lchild->name) < 0)
            return rightRotate(ptr);

        // Right Right Case
        if(bf < -1 && strcasecmp(name, ptr->rchild->name) > 0)
            return leftRotate(ptr);

        // Left Right Case
        if(bf > 1 && strcasecmp(name, ptr->lchild->name) > 0)
        {
            ptr->lchild = leftRotate(ptr->lchild);
            return rightRotate(ptr);
        }

        // Right Left Case
        if(bf < -1 && strcasecmp(name, ptr->rchild->name) < 0)
        {
            ptr -> rchild = rightRotate(ptr->rchild);
            return leftRotate(ptr);
        }
    }
    return ptr;
}

int search(struct node* head, char *name)
{
    while(head != NULL)
    {
        int cmp = strcasecmp(name, head->name);
        if(cmp==0)
        {
            printf("Contact is: %s\n", head->contact);
            return 1;
        }
        else if(cmp<0)
        {
            head = head->lchild;
        }
        else if(cmp>0)
        {
            head = head->rchild;
        }
    }
    return -1;
}

int update(struct node* head, char* name)
{
    char contact[256];
    while(head!=NULL)
    {
        int cmp;
        cmp=strcasecmp(name, head->name);
        if(cmp==0)
        {
            printf("Enter the  new contact\n");
            gets(contact);
            strcpy(head->contact, contact);
            printf("Value updated\n");
            return 1;
        }
        if(cmp<0)
            head=head->lchild;
        if(cmp>0)
            head=head->rchild;
    }
    return 0;
}

struct node* minValueNode(struct node* temp)
{
    struct node* current = temp;

    while(current->lchild != NULL)
    {
        current = current -> lchild;
    }
    return current;
}

struct node* del(struct node* ptr, char* str)
{
    struct node* temp;
    int cmp;
    cmp=strcasecmp(str, ptr->name);

    if(cmp < 0)
    {
        ptr->lchild = del(ptr->lchild, str);
    }
    else if(cmp > 0)
    {
        ptr->rchild = del(ptr->rchild, str);
    }
    else
    {
        if(ptr->lchild == NULL)
        {
            temp = ptr->rchild;
            free(ptr);
            return temp;
        }
        else if(ptr->rchild == NULL)
        {
            temp = ptr->lchild;
            free(ptr);
            return temp;
        }
        else
        {
            temp = minValueNode(ptr->rchild);
            strcpy(ptr->name, temp->name);
            ptr->rchild = del(ptr->rchild, temp->name);
        }
    }

    int bf = getBalanceFactor(ptr);
    // Left Left Imbalance/Case or Right rotation
    if(bf==2 && getBalanceFactor(ptr->lchild) >= 0)
        return rightRotate(ptr);
    // Left Right Imbalance/Case or LR rotation
    else if(bf==2 && getBalanceFactor(ptr->lchild) == -1)
    {
        ptr->lchild = leftRotate(ptr->lchild);
        return rightRotate(ptr);
    }
    // Right Right Imbalance/Case or Left rotation
    else if(bf==-2 && getBalanceFactor(ptr->rchild) <= 0)
        return leftRotate(ptr);
    // Right Left Imbalance/Case or RL rotation
    else if(bf==-2 && getBalanceFactor(ptr->rchild) == 1)
    {
        ptr->rchild = rightRotate(ptr->rchild);
        return leftRotate(ptr);
    }
    return ptr;
}

int main()
{
    char name[50], contact[20];
    struct node* head = NULL;
    int choice, rtrn;
    char c;
    FILE *fp;
    fp = fopen("file.txt", "r");

    char data[100];
    if(fp==NULL)
    {
        printf("Error in opening file\n");
    }
    else
    {
        while(fscanf(fp, "%s %s", name, contact)!=EOF)
        {
            head = insert(name, contact, head);
        }
    }
    fclose(fp);

    do
    {
        printf("\n  PERSONAL PHONE DIRECTORY\n");
        printf("-------------------------------\n");
        printf("\n1. INSERT A NEW CONTACT\n2. DISPLAY ALL CONTACTS\n3. SEARCH CONTACT BY NAME\n4. DELETE A CONTACT\n5. UPDATE CONTACT BY NAME\n6. EXIT\n");
        printf("\n\nENTER OPERATION NUMBER: \n");
        scanf("%d", &choice);
        switch(choice)
        {
        case 1:
            printf("\nEnter name to be inserted in directory: ");
            fflush(stdin);
            gets(name);
            printf("Enter contact number of '%s': ", name);
            gets(contact);
            head = insert(name, contact, head);
            break;

        case 2:
            fp = fopen("file.txt", "w");
            if(head!=NULL)
            {
                system("cls");
                printf("\nContacts present in the directory are:\n");
                inorder(head, fp);
                displayEnd();
            }
            else
                printf("\nOOPS!!! DIRECTORY IS EMPTY .... INSERT NUMBERS FIRST\n");
            break;
        case 3:
            if(head==NULL)
            {
                printf("\nOOPS!!! DIRECTORY IS EMPTY .... INSERT NUMBERS FIRST\n");
            }
            else
            {
                printf("Enter name to find contact number: ");
                fflush(stdin);
                gets(name);
                rtrn = search(head, name);
                if(rtrn==1)
                    printf("CONTACT FOUND!!");
                else
                    printf("OOPS!! CONTACT DOES NOT EXISTS\n");
            }
            break;

        case 4:
            if(head==NULL)
            {
                printf("\nOOPS!!! DIRECTORY IS EMPTY .... INSERT NUMBERS FIRST\n");
            }
            else
            {
                printf("Enter contact to be deleted\n");
                fflush(stdin);
                gets(name);
                rtrn = search(head, name);
                if(rtrn==1)
                    head = del(head, name);
                else
                    printf("\nOOPS!! CONTACT DOES NOT EXISTS\n");

            }
            break;

        case 5:
            if(head==NULL)
            {
                printf("\nOOPS!!! DIRECTORY IS EMPTY .... INSERT NUMBERS FIRST\n");
            }
            else
            {
                printf("Enter name to update its contact Number: \n");
                fflush(stdin);
                gets(name);
                rtrn = update(head, name);
                if(rtrn==1)
                    printf("\n%s updated\n", name);
                else
                    printf("\nOOPS!! CONTACT DOES NOT EXISTS\n");
                break;
            }

        case 6:
            if(head!=NULL)
            {
                system("cls");
                inorder(head, fp);
                displayEnd();
                printf("\nENTER ANY KEY....");
            }
            break;

        default:
            printf("\nOOPS!! YOU HAVE ENTERED INVALID CHOICE\n");

        }
        printf("\nPress any key to continue...");
        getch();
        system("cls");
    }
    while(choice!=6);
    return 0;
}


