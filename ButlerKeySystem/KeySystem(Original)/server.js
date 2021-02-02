const express = require('express');
const path = require('path');
const dotenv =require('dotenv');
const sequelize = require('./config/database');
const schedule = require('node-schedule');
const Person = require('./models/persons');
const Booking = require('./models/bookings');
const Reservation = require('./models/reservations');
const Admin = require('./models/admins');
const Email = require('./controller/emailSend');
const Text = require('./controller/textMessage');
const session = require('express-session');
const passport = require('passport');
const Auth0Strategy = require('passport-auth0');
let app = express();


dotenv.config({ path: './.env'});

app.set('view-engine', 'ejs');


//set up session
var sess ={
    secret: process.env.SESSION_SECRET,
    cookie: {},
    resave: false,
    saveUninitialized: true
};


app.use(session(sess));

var strategy = new Auth0Strategy(
    {
        domain: process.env.AUTH0_DOMAIN,
        clientID: process.env.AUTH0_CLIENT_ID,
        clientSecret: process.env.AUTH0_CLIENT_SECRET,
        callbackURL:
            process.env.AUTH0_CALLBACK_URL
    },
    function(accessToken, refreshToken, extraParams, profile, done) {
        return done(null, profile);
    }
);
passport.use(strategy);

app.use(passport.initialize());
app.use(passport.session());



sequelize.authenticate()
    .then(() => console.log('Database Connected to Sequelize...'))
    .catch(err => console.log('Error: ' + err));

const publicDirectory = path.join(__dirname, './public');
app.use(express.static(publicDirectory));

app.use(express.urlencoded({ extended: true}));
app.use(express.json());

//define routes
app.use('/', require('./routes/pages.js'));
app.use('/keycheckout', require('./routes/form.js'));
app.use('/auth', require('./routes/auth.js'));



var deleteRes = schedule.scheduleJob('59 23 * * *', async function(){ 
    var today= new Date();
    today.setHours(-4, 0, 0);
    await Reservation.destroy({
        where: {
            reservedDate: today
        }
    })
});




var Reminder = schedule.scheduleJob('30 14 * * *', async function(){
    await Booking.aggregate('personID', 'DISTINCT', { plain: false, raw: true, where:{returnedTime: null}})
    .then(async bookings =>{
        if(bookings == undefined || bookings.length == 0){
            return console.log("All Keys are In");
        }
        else{
            for(let i = 0; i < bookings.length; i++){
                await Person.findAll({
                    raw: true,
                    attributes: ['name','phoneNum'],
                    where: {
                        personID: bookings[i].DISTINCT
                    }
                })
                .then(OutPerson =>{
                    Text.textReminder(OutPerson[0].phoneNum);
                })
                .catch(err => console.log(err));
            }
        }
    })
    .catch(err => console.log(err));

});


var EOD = schedule.scheduleJob('30 16 * * *', async function(){
    await Booking.aggregate('personID', 'DISTINCT', { plain: false, raw: true, where:{returnedTime: null}})
    .then(async bookings =>{
        if(bookings == undefined || bookings.length == 0){
            console.log("All Keys are In");
            EmailAdmin("<p>All keys have been returned.</p>");
        }
        else{
            var LatePersonList = [];
            for(let i = 0; i < bookings.length; i++){
                await Person.findAll({
                    raw: true,
                    attributes: ['name','phoneNum'],
                    where: {
                        personID: bookings[i].DISTINCT
                    }
                })
                .then(LatePerson =>{
                    Text.textLate(LatePerson[0].phoneNum);
                    LatePersonList.push(LatePerson[0]);
                })
                .catch(err => console.log(err));
            }
            //make person list formated into  html string
            EmailAdmin("<p>All keys have NOT been returned.</p>");
        }
    })
    .catch(err => console.log(err));
});


async function EmailAdmin(EmailContent){
    await Admin.findAll({
        raw: true,
        attributes: ['email']
    })
    .then(adminList =>{
        for(let i = 0; i < adminList.length; i++){
            Email.emailAdmin(adminList[i].email, EmailContent);
        }
    })
    .catch(err => console.log(err));
}




passport.serializeUser(function (user, done){
    done(null,user);
});

passport.deserializeUser(function (user,done){
    done(null,user);
});


//server info
const PORT = 3000 || process.env.PORT;

app.listen(PORT, () => console.log(`Server running on port ${PORT}`));