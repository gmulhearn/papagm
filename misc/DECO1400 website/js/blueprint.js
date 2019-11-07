

//javascript to be run on main exercise page 2 display box

//make javascript elements visible (originally not visible for progressive enhancement reasons)
document.getElementById('JsOnlyElement').style.display='block';
document.getElementById('help').style.display = "block";
document.getElementById('JsOnlyElement2').style.display='block';

//hide elements that are for non java only
document.getElementById('nonJsOnly').style.display= 'none';
document.getElementById('blueprint-buttons').style.display='none';

//when click to continue is pressed:
document.getElementById('JsOnlyElement').onclick = function() {
	//hide buttons and dialogue and display the blueprint screen with the blue print buttons
	document.getElementById('JsOnlyElement').style.display= 'none';
	document.getElementById('JsOnlyElement2').style.display= 'none';
	document.getElementById('blueprint-buttons').style.display= 'block';
}
//show instructions/dialogue again
document.getElementById('help').onclick = function() {
	document.getElementById('JsOnlyElement').style.display= 'block';
	document.getElementById('JsOnlyElement2').style.display= 'block';
	document.getElementById('blueprint-buttons').style.display= 'none';
}


//colour the blueprint buttons/tiles based on the relevant quiz score in localStorage

//intro color
if (Number(localStorage.exercise_1) == 2){
	document.getElementById('intro-button').style.boxShadow = "0px 0px 29px rgba(0, 255, 0, 1)";
}
else if (Number(localStorage.exercise_1) ==1){
	document.getElementById('intro-button').style.boxShadow = "0px 0px 29px rgb(255,200,0)";
}
else if (Number(localStorage.exercise_1) == 0){
	document.getElementById('intro-button').style.boxShadow = "0px 0px 29px rgb(255,0,0)";
}

//intake color
if (Number(localStorage.exercise_2) == 2){
	document.getElementById('intake-button').style.boxShadow = "0px 0px 29px rgba(0, 255, 0, 1)";
}
else if (Number(localStorage.exercise_2) ==1){
	document.getElementById('intake-button').style.boxShadow = "0px 0px 29px rgb(255,200,0)";
}
else if (Number(localStorage.exercise_2) == 0){
	document.getElementById('intake-button').style.boxShadow = "0px 0px 29px rgb(255,0,0)";
}

//compression color
if (Number(localStorage.exercise_3) == 2){
	document.getElementById('compression-button').style.boxShadow = "0px 0px 29px rgba(0, 255, 0, 1)";
}
else if (Number(localStorage.exercise_3) ==1){
	document.getElementById('compression-button').style.boxShadow = "0px 0px 29px rgb(255,200,0)";
}
else if (Number(localStorage.exercise_3) == 0){
	document.getElementById('compression-button').style.boxShadow = "0px 0px 29px rgb(255,0,0)";
}

//combustion color
if (Number(localStorage.exercise_4) == 2){
	document.getElementById('combustion-button').style.boxShadow = "0px 0px 29px rgba(0, 255, 0, 1)";
}
else if (Number(localStorage.exercise_4) ==1){
    document.getElementById('combustion-button').style.boxShadow = "0px 0px 29px rgb(255,200,0)";
}
else if (Number(localStorage.exercise_4) == 0){
	document.getElementById('combustion-button').style.boxShadow = "0px 0px 29px rgb(255,0,0)";
}

//exhaust color
if (Number(localStorage.exercise_5) == 2){
	document.getElementById('exhaust-button').style.boxShadow = "0px 0px 29px rgba(0, 255, 0, 1)";
}
else if (Number(localStorage.exercise_5) ==1){
	document.getElementById('exhaust-button').style.boxShadow = "0px 0px 29px rgb(255,200,0)";
}
else if (Number(localStorage.exercise_5) == 0){
	document.getElementById('exhaust-button').style.boxShadow = "0px 0px 29px rgb(255,0,0)";
}