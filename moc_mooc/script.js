function getEvents()
{
   var s = "";
   for (var i=0; i < events.length; i++)
   {
     var ev = events[i];
     s += "<h3>" + i  + " ) "; 
     if(ev.hasOwnProperty("event"))
     {
        s += ev.event.name + " [" + ev.event.type + "]";
     }
     else if(ev.hasOwnProperty("username"))
     {
        s += ev.username  +  " [Login]";
     }
     else 
     {
        " ?? ";
     }
     s += "</h3>";
     s += JSON.stringify(ev);
     

   }
   return s;
}



function fillEventList(){ 

   var e = document.getElementById("list");
   e.innerHTML = getEvents();

};
