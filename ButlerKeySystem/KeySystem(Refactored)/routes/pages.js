const express = require('express');
const router = express.Router();
const bodyParser = require('body-parser');
const Helper = require('../controller/helperFunctions');
const QueryList = require('../controller/queries');

router.get('/',(req, res) => {
    res.render('index.ejs');
});

router.get('/codesent',Helper.authenticationMiddleware(),(req, res) => {
    res.render('codesent.ejs');
});

router.get('/inpcheckout',Helper.authenticationMiddleware(),(req, res) => {
    res.render('inpcheckout.ejs');
});

router.get('/questionres',Helper.authenticationMiddleware(),Helper.TimeUpMiddleware(),(req, res) => {
    res.render('questionres.ejs');
});

router.get('/reservekey',Helper.TimeUpMiddleware(),(req, res) => {
    res.render('reservekey.ejs');
});

router.get('/returnkey',Helper.authenticationMiddleware(),(req, res) => {
    res.render('returnkey.ejs');
});

router.get('/allSet',Helper.authenticationMiddleware(),(req, res) => {
    res.render('allset.ejs');
});

router.get('/resConfirm',(req, res) => {
    res.render('resconfirm.ejs');
});


router.get('/admin',Helper.authenticationMiddleware(), (req, res) => {
    res.render('admin.ejs');
});


router.get('/admin/bookings',Helper.authenticationMiddleware(), (req, res) => {
    QueryList.findAllBookings().then(results =>{
        if(results == undefined || results.length == 0){
            res.render('adminBookings.ejs',{bookingsMade: null});
        }
        else{
            res.render('adminBookings.ejs',{bookingsMade: results});
        }
    });
});

router.get('/admin/persons',Helper.authenticationMiddleware(), (req, res) => {
    QueryList.findAllPeople().then(results =>{
        if(results == undefined || results.length == 0){
            res.render('adminPersons.ejs',{personsHere: null});
        }else{
            res.render('adminPersons.ejs',{personsHere: results});
        }
    });
});

router.get('/admin/adminList',Helper.authenticationMiddleware(), (req, res) => {
    QueryList.findAllAdmins().then(results =>{
        if(results == undefined || results.length == 0){
            res.render('adminAdd.ejs',{admins: null});
        }else{
            res.render('adminAdd.ejs',{admins: results});
         }
    });
});

router.get('/admin/keys',Helper.authenticationMiddleware(), (req, res) => {
    QueryList.findAllKeys().then(results =>{
        if(results == undefined || results.length == 0){
            res.render('adminKeys.ejs',{keyList: null});
        }else{
            res.render('adminKeys.ejs',{keyList: results});
        }
    });
});


router.post('/admin/addAdmin',Helper.authenticationMiddleware(), (req, res) => {
    QueryList.createNewAdmin(req.body.newAdminEmail).then(results =>{
        console.log(results);
        res.redirect('/admin/adminList');
    });
});


module.exports = router;