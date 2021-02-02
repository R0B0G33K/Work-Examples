const dotenv = require('dotenv');
dotenv.config({ path: './.env'});
var nodemailer = require('nodemailer');

var transporter = nodemailer.createTransport({
  service: 'gmail',
  auth: {
    user: process.env.EMAIL,
    pass: process.env.EMAIL_PASS
  }
});


module.exports.emailAdmin = function(email, content){
    var mailOptions = {
    from: 'keyboxbutler@gmail.com',
    to: email,
    subject: 'Keys that need to be returned...',
    html: content
    };

    transporter.sendMail(mailOptions, function(error, info){
    if (error) {
        console.log(error);
    } else {
        console.log('Email sent: ' + info.response);
    }
    });
}