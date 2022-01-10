class Hanoi
{
    public static void hanoi(int [] target, int [] bar1, int [] bar2, String tName, String b1Name, String b2Name)
    {
        int disk = 0, index = -1;
        
        for (int i = 0; i < target.length; ++i) {
            disk = (target[i] > 0)?target[i]:0;
            if (disk > 0) {
                index = i;
                target[i] = 0;
                break;
            }
        }

        if (disk == 0 || (tName == "c" && index == 0)) {
            if (index != -1) {
                target[0] = disk;
            }
            return;
        }else{
            for (int i = bar1.length - 1; 0 <= i; --i) {
                if ((bar1[i] == 0 && i == bar1.length - 1) || (bar1[i] == 0 && bar1[i + 1] > disk)){
                    System.out.println(disk + " を" + tName + " から " + b1Name + " に動かす．");
                    bar1[i] = disk;
                    disk = 0;
                    hanoi(target, bar1, bar2, tName, b1Name, b2Name);
                    break;
                }
            }
                
            if (disk > 0) {
                for (int i = bar2.length - 1; 0 <= i; --i) {
                    if ((bar2[i] == 0 && i == bar2.length - 1) || (bar2[i] == 0 && bar2[i + 1] > disk)) {
                        bar2[i] = disk;
                        System.out.println(disk + " を" + tName + " から " + b2Name + " に動かす．");
                        hanoi(bar1, bar2, target, b1Name, b2Name, tName);
                        disk = 0;
                        break;
                    }
                }
            }

            if (disk != 0) {
                target[index] = disk;
            }

            hanoi(bar2, target, bar1, b2Name, tName, b1Name);
        }
    }


    public static void main(String [] args)
    {
        int [] a = {1,2,3,4};
        int [] b = {0,0,0,0};
        int [] c = {0,0,0,0};
        String [] name = {"a", "b", "c"};
        
        System.out.println("4枚の円盤のハノイの塔．");
        hanoi(a, b, c, name[0], name[1], name[2]);
    }
}
