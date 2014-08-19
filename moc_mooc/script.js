function getEvents()
{
   var s = "";
   for (var i=0; i < events.length; i++)
   {
     var ev = events[i];
     s += "<br>" + JSON.stringify(ev);

   }
   return s;
}

function fillEventList(){ 

   var e = document.getElementById("list");
   e.innerHTML = getEvents();

};
