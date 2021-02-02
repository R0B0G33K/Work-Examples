const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');
const Person = require('../models/persons');
const Booking = require('../models/bookings');
const Admin = require('../models/admins');
const Key = require('../models/keyRings');

router.get('/',(req, res) => {
    res.render('index.ejs');
});

router.get('/codesent',authenticationMiddleware(),(req, res) => {
    res.render('codesent.ejs');
});

router.get('/inpcheckout',authenticationMiddleware(),(req, res) => {
    res.render('inpcheckout.ejs');
});

router.get('/questionres',authenticationMiddleware(),TimeUpMiddleware(),(req, res) => {
    res.render('questionres.ejs');
});

router.get('/reservekey',TimeUpMiddleware(),(req, res) => {
    res.render('reservekey.ejs');
});

router.get('/returnkey',authenticationMiddleware(),(req, res) => {
    res.render('returnkey.ejs');
});

router.get('/allSet',authenticationMiddleware(),(req, res) => {
    res.render('allset.ejs');
});

router.get('/resConfirm',(req, res) => {
    res.render('resconfirm.ejs');
});


router.get('/admin',authenticationMiddleware(), (req, res) => {
    res.render('admin.ejs');
});


router.get('/admin/bookings',authenticationMiddleware(), async (req, res) => {
    await Booking.findAll({
        raw: true
    })
    .then(bookings =>{
        if(bookings == undefined || bookings.length == 0){
            res.render('adminBookings.ejs',{bookingsMade: null});
        }else{
            res.render('adminBookings.ejs',{bookingsMade: bookings});
        }
    })
    .catch(err => console.log(err));
});

router.get('/admin/persons',authenticationMiddleware(), async (req, res) => {
    await Person.findAll({
        raw: true
    })
    .then(persons =>{
        if(persons == undefined || persons.length == 0){
            res.render('adminPersons.ejs',{personsHere: null});
        }else{
            res.render('adminPersons.ejs',{personsHere: persons});
        }
    })
    .catch(err => console.log(err));
});

router.get('/admin/adminList',authenticationMiddleware(), async (req, res) => {
    await Admin.findAll({
        raw: true
    })
    .then(adminss =>{
        if(adminss == undefined || adminss.length == 0){
            res.render('adminAdd.ejs',{admins: null});
        }else{
            res.render('adminAdd.ejs',{admins: adminss});
        }
    })
    .catch(err => console.log(err));
});

router.get('/admin/keys',authenticationMiddleware(), async (req, res) => {
    await Key.findAll({
        raw: true
    })
    .then(keylist =>{
        if(keylist == undefined || keylist.length == 0){
            res.render('adminKeys.ejs',{keyList: null});
        }else{
            res.render('adminKeys.ejs',{keyList: keylist});
        }
    })
    .catch(err => console.log(err));
});


router.post('/admin/addAdmin',authenticationMiddleware(), async (req, res) => {
    var adm = await Admin.create({
        email: req.body.newAdminEmail,
    }).then( info =>{
        console.log(info);
        res.redirect('/admin/adminList');
    });
});



function TimeUpMiddleware () {  
    var todayHours = new Date().getHours();
    var todayMin = new Date().getMinutes();
	return (req, res, next) => {
        if (todayHours < 16 || (todayHours == 16 && todayMin <= 30)) return next();
	    res.redirect('/inpcheckout')
	}
}

function authenticationMiddleware () {  
	return (req, res, next) => {

	    if (req.isAuthenticated()) return next();
	    res.redirect('/')
	}
}


module.exports = router;