
module konsol_stl()
{
    scale([1, 1, 1])
      import("konsol.stl", convexity=3);

}

module konsol_joints() 
{
  translate([0,0,0]) {
    rotate([90,0,0]) {
        cylinder(r=10, h=200, center=true);
    }
  }

  color([0,0,1])
  translate([76, 0, 232.5]) {
    rotate([90,0,0]) {
        cylinder(r=10, h=200, center=true);
    }
  }
}

module konsol()
{
 
  translate([30,0,0]) {
   cube([160, 130, 100], center= true);
  }

  translate([100,0,250]) {
   cube([100, 130, 450], center=true);
  }
}

module arm1_stl()
{

 rotate([180, -45 + 18.5 , 0])
 scale([1000, 1000, 1000]) import("Arm1.STL", convexity=3);

//  translate([-1582.5,0, 1842.5]) {
//    mirror([0,0,1]) {
      //import("backhoe-Arm1-1.STL", convexity=3);
//      import("arm1.stl", convexity=3);
//    }
//  }


}

module arm1_joints()
{

  l = 300;
  r = 24;

  translate([0,0,0]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }
  }

  translate([-394.1, 0,  792.56]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }
  }
 // ax arm1 to cyl2  
 translate([-380.5, 0,  1079]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }
  }   

 // arm1 to arm2
 color([1,0,0])
 translate([-1299.72, 0, 1477.5]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }
  }   
 

}

module arm2_stl2()
{

//  translate([-1582.5,0, 1842.5]) {
//    mirror([0,0,1]) {

      import("backhoe-Arm2-1.STL", convexity=3);


//    }
//  }

}


module arm2_stl()
{
    scale([1000, 1000, 1000])
    {
        import("arm2.STL", convexity=3);
    }
}   

module arm2_joints()
{
  l = 300;
  r = 24;

  // arm1 to arm2
  translate([0, 0, 0]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }   
  }
  // arm2  to rod2
  translate([-127.2, 0, 249.4]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }   
  } 

  // arm2  to cyl3 
  translate([-225.78, 0, -583.35 ]) {
    rotate([90,0,0]) {
        cylinder(r=r, h=l, center=true);
    }
  }

}

module arm2_joints_2()
{
// arm2  to skovl
translate([ 137, 0, -1340 ]) {
  rotate([90,0,0]) {
      cylinder(r=20, h=300, center=true);
  }
}

// arm2  to bucket  
translate([ 150, 0, -1440 ]) {
  rotate([90,0,0]) {
      cylinder(r=23, h=300, center=true);
  }
}
}


module arm1() {
    color([1,0,0]){    

     // arm1 close to konsole
     rotate([0,-20,0]) {
      translate([0,0,500]) cube([200, 150, 1100], center=true);
     }
     
     translate([-800,0, 1200]) {
       rotate([0,-60,0]) 
            cube([150, 150, 1200], center=true);
     }
     translate([-400, 0, 950])
     rotate([90,0,0])   
       cylinder(r=200, h=150, center=true);
    }
}


module arm2() {
   color([0,0,1, 0.5]) {

     translate([0,0,-400])  cube([100, 150, 1000], center=true);
    
     translate([80,0,-1000])  rotate([0, -10, 0]) cube([100, 150, 1000], center=true);
     
     translate([-150, 0, -200]) cube([200, 150, 900], center=true);   
   }
}



module skovl_stl2() {
  translate([-1582.5,0,1842.5]){
    mirror([0,0,1]){
      import("skovl-2.stl", convexity=3);
    }
  }
}

module skovl_stl() {
    scale([1000,1000,1000])
    import("skovl.stl", convexity=3);
}

 
module skovl_joints() {

   // skovl to nedre at origin
   rotate([90,0,0]) { cylinder(r=23, h=300, center=true); }

   // arm2 to skovl
   translate([-220, 0, 0]) rotate([90,0,0]) { cylinder(r=23, h=300, center=true); }
}


module skovl() {
 
   color([0,1,0]) {   
     rotate([0,15,0])translate([0, 0, 100]) cube([150, 20, 320], center=true);
   }
}

module shovel_stl()
{
    rotate([-90, 0, 0])
        import("shovel.stl", convexity=3);
}

module shovel_joints()
{
   rotate([90, 0, 0])  cylinder(r=23, h=300, center=true);
   translate([-181, 0, 25]) rotate([90, 0, 0])  cylinder(r=23, h=300, center=true);
}

module shovel()
{
  color([1,0,0, 0.5]){
    //translate([-95,0,0]) cube([250, 150, 120], center=true);
    // translate([0,0,-400]) cube([10, 300, 100 ], center=true);    
    translate([-175, 0, -250]) cube([400, 350, 500], center=true);    
  }
}

module pin(x, z) {
 
 pinr = 10; //24;
 pinh = 350;

 color([1,0,1])
  translate([x *1000, 0, z *1000]){
   rotate([90,0,0]) cylinder (r=pinr, h=pinh, center=true);
  }
}

module nedre_stl() {
    color([1,0,0])
    scale([1000, 1000, 1000])
    import ("nedre.stl", convexity=3);
}


module cyl1_stl()
{
  // cyl1
  scale([1000, 1000, 1000])
    rotate([180, 0,0])
     import ("cylinder1.stl");
}

module rod1_stl()
{
  scale([1000, 1000, 1000])
     import ("rod1.stl");
}


module cyl2_stl()
{
  // cyl1
  scale([1000, 1000, 1000])
     import ("cylinder2.stl");
}


module rod2_stl()
{
  scale([1000, 1000, 1000])
     import ("rod2.stl");
}

module cyl3_stl()
{ 
  scale([1000, 1000, 1000])
     import ("cylinder3.stl");
}

module rod3_stl()
{ 
  scale([1000, 1000, 1000])
     import ("rod3.stl");
}


module nedre()
{
    cube([100,100, 100]);
}


module rod2() {
    color([0.5,0.5,0.5,0.5])
    translate([0,0, 350])cylinder(r=22, h=700, center=true);
}


module rod1()
{
    h = 560;
    color([0.5, 0.5, 0.5])
    translate([0,0, 0.5 * h])cylinder(r=22, h=h, center=true);
}


module rod3()
{
    h = 475;
    color([0.5, 0.5, 0.5])
    translate([0,0, 0.5 * h])cylinder(r=15, h=h, center=true);
}

module cyl1()
{
    r = 50;
    h = 590;
    color([0.5, 0.5, 0.5, 0.5])
    rotate([0,-90,0])translate([0,0, 0.5 * h]) cylinder(r=r, h=h, center=true);
}


module cyl2()
{
    r = 40;
    h = 750;
    color([0.5, 0.5, 0.5, 0.5])
    translate([0,0, 0.5 * h]) cylinder(r=r, h=h, center=true);
}


module cyl3()
{
    r = 40;
    h = 625;
    color([0.5, 0.5, 0.5, 0.5])
    translate([0,0, 0.5 * h]) cylinder(r=r, h=h, center=true);
}


cyl3_stl();
cyl3();




/*

// arm2 to skovl
// pin(-1.29972, 0.1375);

// shovel to nedre
pin(-1.48072, 0.0625);


// nedre to skovl
translate([-1.48072 *1000, 0, 0.0625 *1000]) 
    rotate([180, -124.7499, 0]) {
        pin(0,0); 
        nedre_stl();
    }

// nedre skovl (world space)
pin(-1.438, 0.309);


translate([-1.438 *1000, 0, 1000 *0.309 ]) { 
    rotate([0, -90 + 84.3, 0]) {
        rod3_stl();
    }

}



translate([-1299.72, 0, 1477.5]) {
 // -4.1785349 radians
 translate([-127.2, 0, 249.4])rotate([0, -239.41241431811950, 0])
 {
    pin(0,0);
    rod2_stl();
    rod2();

 }
}



// arm1 to arm2
translate([-1299.72, 0, 1477.5]) {
        rotate([0,-90, 0]) color([1,1,1,0.2])arm2_stl();
        pin(0,0);

    // arm2  to rod2
    translate([-150, 0, 235])
    {
        rotate([0, -239.41241431811950, 0])
            {
                pin(0,0);
                color([1,0,0])rod2_stl();
            }
    }
}

*/

