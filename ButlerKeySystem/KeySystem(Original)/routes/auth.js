const express = require('express');
const router = express.Router();
const passport = require('passport')
const Admin = require('../models/admins');


router.get('/login', passport.authenticate('auth0', {
    scope: 'openid email profile'
  }), function (req, res) {
    res.redirect('/');
  });

router.get('/logout', (req,res) =>{
  console.log('a user has logged out');
    req.logout();
    res.redirect('/');
  });
  
router.get('/callback',  passport.authenticate('auth0'), async (req, res) => {
      console.log('a user has attempted to log in');
      await Admin.findAll({
          raw: true,
          where: {
            email: req.user.emails[0].value
          }
      })
      .then(loggerIn =>{
          if(loggerIn == undefined || loggerIn.length == 0){
            console.log('user did not have permission');
            res.redirect('/logout');
          }else{
            console.log('user had permission');
            res.redirect('/inpcheckout');
          }
      })
      .catch(err => console.log(err));
  });

module.exports = router;