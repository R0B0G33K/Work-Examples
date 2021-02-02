const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');
const bcrypt = require('bcryptjs');
const Key = require('../models/keyRings');
const Person = require('../models/persons');
const Booking = require('../models/bookings');
const Reservation = require('../models/reservations');
const Text = require('../controller/textMessage');

var keyLocations = [];
var message ="";
var timeoutVar;

router.get('/now/:numberKeys',authenticationMiddleware(),TimeUpMiddleware(), async (req, res) => {


    await Key.aggregate('keyLocation', 'DISTINCT', { 
        plain: false, 
        where: {
            available: true
        }
    })
    .then(keys => {
        keyLocations = [];
        keys.forEach(fixFormat)
        //put into orgs
    })
    .catch(err => console.log(err));



    if(Number(req.params.numberKeys) >= 1 && Number(req.params.numberKeys) <= 10){
    res.render('keycheckout.ejs', {arraylist: keyLocations, numberKeys: Number(req.params.numberKeys), type: 'now', message: message});
    }
});



router.get('/res/:numberKeys', async (req, res) => {


    await Key.aggregate('keyLocation', 'DISTINCT', { 
        plain: false,
    })
    .then(keys => {
        keyLocations = [];
        keys.forEach(fixFormat)
        //put into orgs
    })
    .catch(err => console.log(err));
    


    if(Number(req.params.numberKeys) >= 1 && Number(req.params.numberKeys) <= 10){
    res.render('keycheckout.ejs', {arraylist: keyLocations, numberKeys: Number(req.params.numberKeys), type: 'res', message: message});
    }
});




router.post('/checkres',async (req, res) => {

    var today = new Date(new Date().getFullYear(),new Date().getMonth() , new Date().getDate())
    today.setHours(-4, 0, 0);
    await Person.findAll({
        raw: true,
        attributes: ['personID','company','name','signiture'],
        where: {
            phoneNum:  req.body.pnum
        }
    })
    .then(async possiblePerson =>{
        if(possiblePerson == undefined || possiblePerson.length == 0 || possiblePerson.length > 1){
            return console.log('error no people');
        }
        else{
            await Reservation.findAll({
                raw: true,
                where: {
                    personID:  possiblePerson[0].personID,
                    reservedDate: today
                }
            })
            .then(async reservations =>{
                if(reservations == undefined || reservations == 0){
                    return console.log('error no reservations');
                }
                else{
                    for(var i=0; i<reservations.length; i++){
                        await Key.findAll({
                            raw: true,
                            attributes: ['keyNum'],
                            where: {
                                keyLocation:  reservations[i].keyLocation,
                                available: true
                            }
                        })
                        .then(async openKey =>{
                            currentKeyNum = null;
                            currentKeyNum = openKey[0].keyNum;
                            //Done: update key availibility
                            await Key.update(
                                {
                                    available: false
                                },
                                {
                                where: {
                                    keyNum: currentKeyNum
                                }
                            }
                            ).then(function() {
                                console.log('Key '+currentKeyNum+' has been checked out')
                            }).catch(err => console.log(err));
                        })
                        .catch(err => console.log(err));
                        
                        var book = await Booking.create({
                            keyNum: currentKeyNum,
                            personID: reservations[i].personID,
                            reason: reservations[i].reason,
                            attendent: null
                        }); 

                        await Reservation.destroy({
                            where: {
                                keyLocation: reservations[i].keyLocation,
                                personID: reservations[i].personID,
                                reservedDate: today
                            }
                        })
                    }  
                    res.redirect('/keycheckout/review/res/'+req.body.pnum+'');
                }
            })
            .catch(err => console.log(err));
        }
    })
    .catch(err => console.log(err));
});

router.get('/review/res/:phonenumber', async (req, res) => {
        await Person.findAll({
            raw: true,
            attributes: ['personID','company','name','signiture'],
            where: {
                phoneNum:  req.params.phonenumber
            }
        })
        .then(async possiblePerson =>{
            if(possiblePerson == undefined || possiblePerson.length == 0 || possiblePerson.length > 1){
                return console.log('error');
            }
            else{
                var attendentType;
                if(req.params.type == 'return'){
                    attendentType = 'attendent2'
                }
                else{
                    attendentType = 'attendent'
                }
                //find all bookings of found person
                await Booking.findAll({
                    raw: true,
                    attributes: ['keyNum','predictedTime'],
                    where: {
                        personID:  possiblePerson[0].personID,
                        [attendentType]: null
                    }
                })
                .then(keysout =>{
                    if(keysout == undefined || keysout== 0){
                        return console.log('error');
                    }
                    else{
                        console.log(keysout)
                        clearTimeout(timeoutVar);
                        res.render('review.ejs', { person: possiblePerson[0], keys: keysout, type: "res"});
                    }
         
                })
                .catch(err => console.log(err));
            }
        })
        .catch(err => console.log(err));
});


router.get('/review/nores/:phonenumber',authenticationMiddleware(),TimeUpMiddleware(), async (req, res) => {
    await Person.findAll({
        raw: true,
        attributes: ['personID','company','name','signiture'],
        where: {
            phoneNum:  req.params.phonenumber
        }
    })
    .then(async possiblePerson =>{
        if(possiblePerson == undefined || possiblePerson.length == 0 || possiblePerson.length > 1){
            return console.log('error');
        }
        else{
            var attendentType = 'attendent';
            
            //find all bookings of found person
            await Booking.findAll({
                raw: true,
                attributes: ['keyNum','predictedTime'],
                where: {
                    personID:  possiblePerson[0].personID,
                    [attendentType]: null
                }
            })
            .then(keysout =>{
                if(keysout == undefined || keysout== 0){
                    return console.log('error');
                }
                else{
                    console.log(keysout)
                    clearTimeout(timeoutVar);
                    res.render('review.ejs', { person: possiblePerson[0], keys: keysout, type: "now"});
                }
     
            })
            .catch(err => console.log(err));
        }
    })
    .catch(err => console.log(err));
});


router.get('/review/return/:phonenumber',authenticationMiddleware(),TimeUpMiddleware(), async (req, res) => {
    await Person.findAll({
        raw: true,
        attributes: ['personID','company','name','signiture'],
        where: {
            phoneNum:  req.params.phonenumber
        }
    })
    .then(async possiblePerson =>{
        if(possiblePerson == undefined || possiblePerson.length == 0 || possiblePerson.length > 1){
            return console.log('error');
        }
        else{
            var attendentType = 'attendent2';

            //find all bookings of found person
            await Booking.findAll({
                raw: true,
                attributes: ['keyNum','predictedTime'],
                where: {
                    personID:  possiblePerson[0].personID,
                    [attendentType]: null
                }
            })
            .then(keysout =>{
                if(keysout == undefined || keysout== 0){
                    return console.log('error');
                }
                else{
                    console.log(keysout)
                    clearTimeout(timeoutVar);
                    res.render('review.ejs', { person: possiblePerson[0], keys: keysout, type: "return"});
                }
     
            })
            .catch(err => console.log(err));
        }
    })
    .catch(err => console.log(err));
});


router.post('/:type/complete',authenticationMiddleware(), async (req,res) =>{
    if (req.params.type == 'return'){
        var tempcurdate = new Date();
        const curdate = tempcurdate;
        await Booking.update(
            {
                returnedTime: curdate,
                attendent2: req.body.attend2
            },
            {
            where: {
                personID:  req.body.personID,
                attendent2: null
            }
        }
        ).then(async function() {
            console.log('added sign out attendent');
            await Booking.findAll({
                raw: true,
                attributes: ['keyNum'],
                where: {
                    personID:  req.body.personID,
                    returnedTime: curdate
                }
            })
            .then(async keysin =>{
                if(keysin == undefined || keysin== 0){
                    return console.log('error');
                }
                else{
                    for(var i=0; i<keysin.length; i++){
                        await Key.update(
                            {
                                available: true
                            },
                            {
                            where: {
                                keyNum: keysin[i].keyNum
                            }
                        }
                        ).then(function() {
                            console.log('Key '+keysin[i].keyNum+' has been checked in')
                        }).catch(err => console.log(err));
                    }
                    res.redirect('/allSet');
                }
            })
            .catch(err => console.log(err));
        }).catch(err => console.log(err));

    }
    else if(req.params.type == 'res'){
        await Booking.update(
            {
                attendent: req.body.attend,
                predictedTime: req.body.returnTime
            },
            {
            where: {
                personID:  req.body.personID,
                attendent: null,
                predictedTime: null
            }
        }
        ).then(function() {
            console.log('added sign in attendent and predicted time');
            res.redirect('/allSet');
        }).catch(err => console.log(err));
    }
    else{
        await Booking.update(
            {
                attendent: req.body.attend
            },
            {
            where: {
                personID:  req.body.personID,
                attendent: null
            }
        }
        ).then(function() {
            console.log('added sign in attendent');
            res.redirect('/allSet');
        }).catch(err => console.log(err));        
    }
});

router.post('/return', authenticationMiddleware(), (req,res) =>{
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

router.post('/now/:numberKeys/submit',authenticationMiddleware(),TimeUpMiddleware(), async (req, res) => {
        killUnsigned();
        //DONE: check if person exists and update if they do
        var person = null;
        await Person.findAll({
            raw: true,
            attributes: ['personID'],
            where: {
                phoneNum:  req.body.phoneNum
            }
        })
        .then(async possiblePerson =>{
            if(possiblePerson == undefined || possiblePerson.length == 0){
                console.log(possiblePerson);
                const per = await Person.create({
                    name: req.body.name,
                    phoneNum: req.body.phoneNum,
                    company: req.body.company,
                    sponsName: req.body.sponsName,
                    sponsEmail: req.body.sponsEmail,
                    sponsPhoneNum: req.body.sponsPhoneNum,
                    signiture: req.body.signiture
                });
                person = per.personID;

            }
            else{
                await Person.update(
                    {
                        name: req.body.name,
                        phoneNum: req.body.phoneNum,
                        company: req.body.company,
                        sponsName: req.body.sponsName,
                        sponsEmail: req.body.sponsEmail,
                        sponsPhoneNum: req.body.sponsPhoneNum,
                        signiture: req.body.signiture
                    },
                    {
                    where: {
                        personID: possiblePerson[0].personID
                    }
                }
                ).then(function() {
                    person = possiblePerson[0].personID;
                }).catch(err => console.log(err));
            }
        })
        .catch(err => console.log(err));


        //TO DO: make sure no same key locations
        //DONE: make sure key time is not prior to pickup and not past 4:30
        for(let i = 1; i < Number(req.params.numberKeys)+1; i++){
            var currentKeyNum;
            var currentKeyLocation ='key'+i+'Location';
            var currentKeyTime = 'key'+i+'ReturnTime';
            var currentKeyReason = 'key'+i+'Purpose';

            await Key.findAll({
                raw: true,
                attributes: ['keyNum'],
                where: {
                    keyLocation:  req.body[currentKeyLocation],
                    available: true
                }
            })
            .then(async openKey =>{
                currentKeyNum = null;
                currentKeyNum = openKey[0].keyNum;
                //Done: update key availibility
                await Key.update(
                    {
                        available: false
                    },
                    {
                    where: {
                        keyNum: currentKeyNum
                    }
                }
                ).then(function() {
                    console.log('Key '+currentKeyNum+' has been checked out')
                }).catch(err => console.log(err));
            })
            .catch(err => console.log(err));
            
            var book = await Booking.create({
                keyNum: currentKeyNum,
                personID: person,
                predictedTime: req.body[currentKeyTime],
                reason: req.body[currentKeyReason],
                attendent: null
            });   
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
        
        await Person.findAll({
            raw: true,
            attributes: ['personID'],
            where: {
                phoneNum:  req.body.phoneNum
            }
        })
        .then(async possiblePerson =>{
            if(possiblePerson == undefined || possiblePerson.length == 0){
                console.log(possiblePerson);
                const per = await Person.create({
                    name: req.body.name,
                    phoneNum: req.body.phoneNum,
                    company: req.body.company,
                    sponsName: req.body.sponsName,
                    sponsEmail: req.body.sponsEmail,
                    sponsPhoneNum: req.body.sponsPhoneNum,
                    signiture: req.body.signiture
                });
                person = per.personID;

            }
            else{
                await Person.update(
                    {
                        name: req.body.name,
                        phoneNum: req.body.phoneNum,
                        company: req.body.company,
                        sponsName: req.body.sponsName,
                        sponsEmail: req.body.sponsEmail,
                        sponsPhoneNum: req.body.sponsPhoneNum,
                        signiture: req.body.signiture
                    },
                    {
                    where: {
                        personID: possiblePerson[0].personID
                    }
                }
                ).then(function() {
                    person = possiblePerson[0].personID;
                }).catch(err => console.log(err));
            }
        })
        .catch(err => console.log(err));
        console.log(req.body.sresdate);
        var dateArray = getDatesBetweenDates(req.body.sresdate, req.body.eresdate);
        console.log(dateArray);
        for(let j = 0; j < dateArray.length; j++){
            for(let i = 1; i < Number(req.params.numberKeys)+1; i++){
                var currentKeyNum;
                var currentKeyLocation ='key'+i+'Location';
                var currentKeyReason = 'key'+i+'Purpose';
                var reserve = await Reservation.create({
                    keyLocation: req.body[currentKeyLocation],
                    personID: person,
                    reservedDate: dateArray[j],
                    reason: req.body[currentKeyReason],
                });
            }
        } 
        console.log('Great Scott!');
        //do next

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


function fixFormat(item,index){
    keyLocations[index] = item.DISTINCT;
}

async function killUnsigned(){


    await Booking.findAll({
        raw: true,
        attributes: ['keyNum'],
        where: {
            attendent: null
        }
    })
    .then(async keysin =>{
        if(keysin == undefined || keysin== 0){
            return console.log('error');
        }
        else{
            for(var i=0; i<keysin.length; i++){
                await Key.update(
                    {
                        available: true
                    },
                    {
                    where: {
                        keyNum: keysin[i].keyNum
                    }
                }
                ).then(function() {
                    console.log('Key '+keysin[i].keyNum+' has been checked in')
                }).catch(err => console.log(err));
            }
        }
    })
    .catch(err => console.log(err));

    
    await Booking.destroy({
        where: {
            attendent: null
        }
    })

    console.log('Deleted all unsigned');
}

async function killPNum(pnum){
    await Person.destroy({
        where: {
            phoneNum: pnum
        }
    })
    console.log('Deleted '+pnum+'');
}

const getDatesBetweenDates = (startDate, endDate) => {
    let dates = []
    //to avoid modifying the original date
    const theDate = new Date(startDate)
    const theEDate = new Date(endDate)
    console.log(theDate);
    while (theDate < theEDate) {
      dates = [...dates, new Date(theDate)]
      theDate.setDate(theDate.getDate() + 1)
    }
    dates = [...dates, theEDate]
    return dates
}


function authenticationMiddleware () {  
	return (req, res, next) => {

	    if (req.isAuthenticated()) return next();
	    res.redirect('/')
	}
}

function TimeUpMiddleware () {  
    var todayHours = new Date().getHours();
    var todayMin = new Date().getMinutes();
	return (req, res, next) => {
        if (todayHours < 16 || (todayHours == 16 && todayMin <= 30)) return next();
	    res.redirect('/inpcheckout')
	}
}


module.exports = router;