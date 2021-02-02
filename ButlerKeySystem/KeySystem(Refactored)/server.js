const express = require('express');
const path = require('path');
const dotenv =require('dotenv');
const sequelize = require('./config/database');
const schedule = require('node-schedule');
const QueryList = require('./controller/queries');
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
    //destroy reservation when expired
    QueryList.destroyReservationWhenOutDate(today);
});




var Reminder = schedule.scheduleJob('30 14 * * *', async function(){
    QueryList.bookingsNotRetuned().then(results =>{
        if(results == undefined || results.length == 0){
            return console.log("All Keys are In");
        }
        else{
            for(let i = 0; i < bookings.length; i++){
                //find person name and phone number by ID
                QueryList.findPersonByID(results[i].DISTINCT).then(results =>{
                    Text.textReminder(results[0].phoneNum);
                });
            }
        }
    });
});


var EOD = schedule.scheduleJob('30 16 * * *', async function(){
    QueryList.bookingsNotRetuned().then(results =>{
        if(results == undefined || results.length == 0){
            console.log("All Keys are In");
            EmailAdmin("<p>All keys have been returned.</p>");
        }
        else{
            var LatePersonList = [];
            for(let i = 0; i < bookings.length; i++){
                //find person name and phone number by ID
                QueryList.findPersonByID(results[i].DISTINCT).then(results =>{
                    Text.textLate(results[0].phoneNum);
                    LatePersonList.push(results[0]);
                });
            }
            //make person list formated into  html string
            EmailAdmin("<p>All keys have NOT been returned.</p>");
        }
    });
});


async function EmailAdmin(EmailContent){
    QueryList.findAllAdmins().then(results =>{
        for(let i = 0; i < results.length; i++){
            Email.emailAdmin(results[i].email, EmailContent);
        }
    })
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