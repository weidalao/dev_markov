#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

// Module Stuff
MODULE_LICENSE("Apache"); // Change this to "GPL" if you get annoyed about
                          // the kernal playing a crying fit about non GPL stuff
MODULE_DESCRIPTION("A Markov device driver.");
MODULE_AUTHOR("Ben Cartwright-Cox");

static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

// Adding callbacks into the gig
static struct file_operations fops =
{
    .read = dev_read,
    .write = dev_write,
    .open = dev_open,
    .release = dev_rls,
};

static struct MKovEnt
{
    int times;
    char word[20];
};

static int RollingLimit = 0;

static struct MKovEnt Words[1024] = {};

int init_module(void) {
    int t = register_chrdev(89,"mkov",&fops);
    if(t<0) {
        printk(KERN_ALERT "MKOV MODULE COULD NOT INIT AAAAAAAAA");
    } else {
        printk(KERN_ALERT "mkov module init'd. Insanity loaded into kernel. Good job hero.");
    }

    return t;
}

void cleanup_module(void) {
    unregister_chrdev(89,"mkov");
}

static int dev_open(struct inode *inod,struct file *fil) {
    return 0; // Who actually cares?!
}

static ssize_t dev_read(struct file *foole,char *buff,size_t len,loff_t *off) {
    // short count = 0;
    // while (len && (msg[readPos]!=0))
    // {
    //     put_user(msg[readPos],buff++); //copy byte from kernel space to user space
    //     count++;
    //     len--;
    //     readPos++;
    // }
    // return count;
    return 0;
}

static char msg[20]={0};
static int WordSize = 0;
static short readPos=0;

static ssize_t dev_write(struct file *foole,const char *buff,size_t len,loff_t *off) {

    while(len > 0) {
        // what you want is in buff[len]
        char letter = buff[len];
        if(letter == 0x2E || letter == 0x20 || letter == 0x2C || letter == 0x0D || letter == 0x0A ) {
            // Check how much is in the word buffer
            if(WordSize == 0) {
                // Then this is useless
                continue;
            } else {
                int i; // C99 mode
                int j; // C99 mode
            // Then check if the word is in the system already
                int foundit = 0;
                for (i = 0; i < 1023; ++i) {
                    int correctwordmaybe = 1;
                    for (j = 0; j < 19; ++j) {
                        if (Words[i].word[j] == msg[j]) {
                            correctwordmaybe = 0;
                        }
                    }
                    if(correctwordmaybe) {
                        // If it is then increment it.
                        Words[i].times++;
                        foundit = 1;
                        break;
                    }
                }
                if(foundit == 0) {
                    RollingLimit++;
                    if(RollingLimit == 1024) {
                        RollingLimit = 0;
                    }
                    
                    int i;
                    for (i = 0; i < 19; ++i)
                    {
                        Words[RollingLimit].word[i] = msg[i];
                    }
                    Words[RollingLimit].times = 0;
                }
                // If not add it
            }

            // Then set the latest word var
        }
        len--;
    }

    return 0; // See above

}

static int dev_rls(struct inode *inod,struct file *fil) {
    return 0;
}