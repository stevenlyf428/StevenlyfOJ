public class testjava {
    public static void main(String[] args) {
        int age = 23;
        char gender = 'M';
        boolean married = false;

        int num = 500;
        if (age < 25 && gender == 'M' && married == false) {
            num += 100;
        }
        if (gender == 'F' || married == true) {
            num -= 200;
        }
        if (age > 45 && age < 65) {
            num -= 100;
        }
        if (age < 16 || age > 65 || gender != 'F' && gender != 'M')
            num = 0;
        System.out.println(num);
    }

}