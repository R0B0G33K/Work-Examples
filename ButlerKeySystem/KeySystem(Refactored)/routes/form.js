const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');
const bcrypt = require('bcryptjs');
const QueryList = require('../controller/queries');
const Text = require('../controller/textMessage');
const Helper = require('../controller/helperFunctions');

var message ="";
var timeoutVar;

router.get('/now/:numberKeys',Helper.authenticationMiddleware(),Helper.TimeUpMiddleware(), async (req, res) => {
    //finds all key locations availble (distinct)
    QueryList.findUniqueKeyLocationsAvailable().then(results => {
        if(Number(req.params.numberKeys) >= 1 && Number(req.params.numberKeys) <= 10){
            res.render('keycheckout.ejs', {arraylist: results, numberKeys: Number(req.params.numberKeys), type: 'now', message: message});
        }
    });
});



router.get('/res/:numberKeys', async (req, res) => {
    //finds all key locations availble (distinct)
    QueryList.findUniqueKeyLocationsAvailable().then(results => {
        if(Number(req.params.numberKeys) >= 1 && Number(req.params.numberKeys) <= 10){
            res.render('keycheckout.ejs', {arraylist: results, numberKeys: Number(req.params.numberKeys), type: 'res', message: message});
        }
    });
});



router.post('/checkres', (req, res) => {

    var today = new Date(new Date().getFullYear(),new Date().getMonth() , new Date().getDate())
    today.setHours(-4, 0, 0);

    //find all people with entered phone number return (ID, company, name, signiture)
    QueryList.findPersonWithPhoneNum(req.body.pnum).then(results =>{
        if(results == undefined || results.length == 0 || results.length > 1){
            return console.log('error no people');
        }
        else{
            //find all reservations with ID and date
            QueryList.findAllReservationsForThisIDAndDate(results[0].personID, today).then(results =>{
                if(results == undefined || results == 0){
                    return console.log('error no reservations');
                }
                else{
                    //find all key numbers available for every reserved location
                    for(var i=0; i<results.length; i++){
                        var currentKey = null;
                        QueryList.findAvailableKeyNumbersForALocation(results[i].keyLocation).then(results =>{
                            currentKey = results[0].keyNum
                            //update availibility of key by key number
                            QueryList.updateKeyAvailability(false,results[0].keyNum);
                        });
                        //create a booking
                        QueryList.createBooking(currentKey,results[i].personID,null,results[i].reason,null);
                        //destroy reservation
                        QueryList.destroyReservation(results[i].keyLocation,results[i].personID, today);
                    }
                    res.redirect('/keycheckout/review/res/'+req.body.pnum+'');
                }
            });
        }
    });
});

router.get('/review/res/:phonenumber', (req, res) => {
    //find all people with entered phone number return (ID, company, name, signiture)
    QueryList.findPersonWithPhoneNum(req.params.phonenumber).then(results =>{
        if(results == undefined || results.length == 0 || results.length > 1){
            return console.log('error no people');
        }
        else{
            var attendentType;
            if(req.params.type == 'return'){
                attendentType = 'attendent2'
            }
            else{
                attendentType = 'attendent'
            }
            //find all bookings with this personID, specify attendent should be null
            QueryList.findAllBookingsByOneID(results[0].personID, attendentType).then(results =>{
                if(results == undefined || results== 0){
                    return console.log('error');
                }
                else{
                    console.log(results)
                    clearTimeout(timeoutVar);
                    res.render('review.ejs', { person: possiblePerson[0], keys: results, type: "res"});
                }
            });
        }
    });
});


router.get('/review/nores/:phonenumber',Helper.authenticationMiddleware(),Helper.TimeUpMiddleware(), (req, res) => {
    //find all people with entered phone number return (ID, company, name, signiture)
    QueryList.findPersonWithPhoneNum(req.params.phonenumber).then(results =>{
        if(results == undefined || results.length == 0 || results.length > 1){
            return console.log('error no people');
        }
        else{
             var attendentType = 'attendent'
            //find all bookings with this personID, specify attendent should be null
            QueryList.findAllBookingsByOneID(results[0].personID, attendentType).then(results =>{
                if(results == undefined || results== 0){
                    return console.log('error');
                }
                else{
                    console.log(results)
                    clearTimeout(timeoutVar);
                    res.render('review.ejs', { person: possiblePerson[0], keys: results, type: "now"});
                }
            });
        }
    });
});


router.get('/review/return/:phonenumber',Helper.authenticationMiddleware(),Helper.TimeUpMiddleware(), (req, res) => {
    //find all people with entered phone number return (ID, company, name, signiture)
    QueryList.findPersonWithPhoneNum(req.params.phonenumber).then(results =>{
        if(results == undefined || results.length == 0 || results.length > 1){
            return console.log('error no people');
        }
        else{
             var attendentType = 'attendent2'
            //find all bookings with this personID, specify attendent should be null
            QueryList.findAllBookingsByOneID(results[0].personID, attendentType).then(results =>{
                if(results == undefined || results== 0){
                    return console.log('error');
                }
                else{
                    console.log(results)
                    clearTimeout(timeoutVar);
                    res.render('review.ejs', { person: possiblePerson[0], keys: results, type: "return"});
                }
            });
        }
    });
});


router.post('/:type/complete',Helper.authenticationMiddleware(), (req,res) =>{
    if (req.params.type == 'return'){
        var tempcurdate = new Date();
        const curdate = tempcurdate;
        //update bookings on return
        QueryList.updateBookingsOnReturn(curdate,req.body.attend2, req.body.personID).then(function(){
            //find all bookings key numbers by ID and return time
            QueryList.findAllBookingKeyNumbersByIDAndReturnTime(req.body.personID, curdate).then(results =>{
                if(results == undefined || results == 0){
                    return console.log('error');
                }
                else{
                    for(var i=0; i<results.length; i++){
                        //update availibility of key by key number
                        QueryList.updateKeyAvailability(true,results[i].keyNum);
                    }
                    res.redirect('/allSet');
                }
            })
        });
    }
    else if(req.params.type == 'res'){
        //update booking on reservation checkout
        QueryList.updateBookingsOnReservationCheckout(req.body.attend, req.body.returnTime, req.body.personID).then(function(){
            res.redirect('/allSet');
        });
    }
    else{
        //Update all bookings by ID sign out attendents
        QueryList.updateBookingsSignOutAttendent(req.body.attend, req.body.personID).then(function(){
            res.redirect('/allSet');   
        });
    }
});

router.post('/return', Helper.authenticationMiddleware(), (req,res) =>{
    console.log('return');
    res.redirect('/keycheckout/review/return/'+req.body.pnum+'');
});



router.post('/:type/compare', async (req,res) =>{
    console.log('compare');
    if(await bcrypt.compare(req.body.code, req.body.vcode)){
        //correct
        if(req.params.type == 'res'){
            res.redirect('/resConfirm');
        }
        else{
            res.redirect('/keycheckout/review/nores/'+req.body.pnum+'');
        }
    }
    else{
        //wrong 
        res.render('codesent.ejs', { verification: req.body.vcode, phoneNumber: req.body.pnum, type: req.params.type});
    }
});

router.post('/now/:numberKeys/submit',Helper.authenticationMiddleware(),Helper.TimeUpMiddleware(), async (req, res) => {
        killUnsigned();
        //DONE: check if person exists and update if they do
        var person = null;
        //find all people with entered phone number return (ID, company, name, signiture)
        QueryList.findPersonWithPhoneNum(req.body.phoneNum).then(results =>{
            if(results == undefined || results.length == 0){
                //create new person returns ID of new person
                QueryList.createPerson(req.body.name, req.body.phoneNum, req.body.company, req.body.sponsName, req.body.sponsEmail, req.body.sponsPhoneNum, req.body.signiture).then(results =>{
                    person = results[0];
                });
            }
            else{
                //Update person info with ID, return ID again
                QueryList.updatePersonByID(results[0].personID,req.body.name, req.body.phoneNum, req.body.company, req.body.sponsName, req.body.sponsEmail, req.body.sponsPhoneNum, req.body.signiture).then(results =>{
                    person = results[0];
                });
            }
        });
        //TO DO: make sure no same key locations
        //DONE: make sure key time is not prior to pickup and not past 4:30
        for(let i = 1; i < Number(req.params.numberKeys)+1; i++){
            var currentKeyNum;
            var currentKeyLocation ='key'+i+'Location';
            var currentKeyTime = 'key'+i+'ReturnTime';
            var currentKeyReason = 'key'+i+'Purpose';

            //find all key numbers available for a location
            QueryList.findAvailableKeyNumbersForALocation(req.body[currentKeyLocation]).then(results =>{
                currentKeyNum = null;
                currentKeyNum = results.keyNum;
                //update availibility of key by key number
                QueryList.updateKeyAvailability(false, currentKeyNum).then(function(){
                    console.log('Key '+currentKeyNum+' has been checked out');
                });
            });
            //create a booking
            QueryList.createBooking(currentKeyNum, person, req.body[currentKeyTime], req.body[currentKeyReason], null);   
        }
        var varNum = Text.textConfirm(req.body.phoneNum);
        var phoneNum = req.body.phoneNum;
        var saltRounds = 8;
        //ENCRYPT VAR NUM
        //const hashedNum = await bcrypt.hash(varNum, 8);
        bcrypt.hash(varNum.toString(), saltRounds, function(err, hash) {
            if (err) {
                throw err;
            }
            timeoutVar = setTimeout(killUnsigned, 300000);  
            return res.render('codesent.ejs', { verification: hash, phoneNumber: phoneNum, type: 'nores'});
        });
        //signiture back person.signiture.toString('utf-8')
    });

router.post('/res/:numberKeys/submit', async (req, res) => {

    var person = null;
    //find all people with entered phone number return (ID, company, name, signiture)
    QueryList.findPersonWithPhoneNum(req.body.phoneNum).then(results =>{
        if(results == undefined || results.length == 0){
            //create new person returns ID of new person
            QueryList.createPerson(req.body.name, req.body.phoneNum, req.body.company, req.body.sponsName, req.body.sponsEmail, req.body.sponsPhoneNum, req.body.signiture).then(results =>{
                person = results[0];
            });
        }
        else{
            //Update person info with ID, return ID again
            QueryList.updatePersonByID(results[0].personID,req.body.name, req.body.phoneNum, req.body.company, req.body.sponsName, req.body.sponsEmail, req.body.sponsPhoneNum, req.body.signiture).then(results =>{
                person = results[0];
            });
        }
    });
    console.log(req.body.sresdate);
    var dateArray = Helper.getDatesBetweenDates(req.body.sresdate, req.body.eresdate);
    console.log(dateArray);
    for(let j = 0; j < dateArray.length; j++){
        for(let i = 1; i < Number(req.params.numberKeys)+1; i++){
            var currentKeyLocation ='key'+i+'Location';
            var currentKeyReason = 'key'+i+'Purpose';
            //creates a new reservation
            QueryList.createReservation(req.body[currentKeyLocation], person, dateArray[j], req.body[currentKeyReason]);
        }
    } 
    var varNum = Text.textConfirm(req.body.phoneNum);
    var phoneNum = req.body.phoneNum;
    var saltRounds = 8;
    //ENCRYPT VAR NUM
    //const hashedNum = await bcrypt.hash(varNum, 8);
    bcrypt.hash(varNum.toString(), saltRounds, function(err, hash) {
        if (err) {
            throw err;
        }  
        return res.render('codesent.ejs', { verification: hash, phoneNumber: phoneNum, type: 'res'});
    });
});


function killUnsigned(){
    //find all bookings with no attendent (prep for deletion)
    QueryList.findAllBookingsWhereAttendentNull().then(results =>{
        if(results == undefined || results== 0){
            return console.log('error');
        }
        else{
            for(var i=0; i<keysin.length; i++){
                //update availibility of key by key number
                QueryList.updateKeyAvailability(true, results[i].keyNum)
                .then(function() {
                    console.log('Key '+results[i].keyNum+' has been checked in')
                }).catch(err => console.log(err));
            }
        }
    });
    //destroy timed out bookings
    QueryList.destroyBookingsWithNoAttendent();
    console.log('Deleted all unsigned');
}


module.exports = router;