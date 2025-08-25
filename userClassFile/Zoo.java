class Animal {
    int age;
    String name;
    Animal(){
        age = 0;
        name = "No name";
    }
    Animal(int age, String name){
        this.age = age;
        this.name = name;
    }

    @Override
    public String toString(){
        return "(" + name + "," + age+"岁)";
    }

    public String eat(){
        return "吃东西";
    }
}

class Dog extends Animal{
    Dog(int age, String name){
        super(age, name);
    }
    @Override
    public String eat(){
        return "吃骨头";
    }

    @Override
    public String toString(){
        return "小狗" + super.toString();
    }
}

class Cat extends Animal{
    Cat(int age, String name){
        super(age, name);
    }
    @Override
    public String eat(){
        return "吃老鼠";
    }

    @Override
    public String toString(){
        return "小猫" + super.toString();
    }
}

interface Language{
    public String speak();
}

class Human extends Animal implements Language{
    Human(int age, String name){
        super(age, name);
    }
    @Override
    public String eat(){
        return "吃米饭";
    }

    @Override
    public String toString(){
        return "小人" + super.toString();
    }

    @Override
    public String speak(){
        return "汉语";
    }
}

public class Zoo {
    public static void main(String[] args) {
        Animal[] zoo=new Animal[5];
        zoo[0]=new Dog(2,"旺财");
        zoo[1]=new Dog(1,"黑子");
        zoo[2]=new Cat(3,"花花");
        zoo[3]=new Cat(4,"美猫");
        zoo[4]=new Human(9,"小王");
        for(Animal a:zoo){
            myIO.println(a.toString() + a.eat());
            //System.out.println(a.toString() + a.eat());
        }
        myIO.println("小王说：" + ((Human)zoo[4]).speak());
        //System.out.println("小王说：" + ((Human)zoo[4]).speak());
    }
}
