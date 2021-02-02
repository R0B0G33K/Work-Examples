const dotenv = require('dotenv');
dotenv.config({ path: './.env'});
var twilio = require('twilio');
var client = new twilio(process.env.TWILIO_ACCOUNT_SID, process.env.TWILIO_AUTH_TOKEN);

module.exports.textConfirm = function(phoneNumber){
    var code = Math.floor(100000+Math.random()*900000);
    client.messages.create({
        to: phoneNumber,
        from: process.env.YOUR_TWILIO_NUMBER,
        body: 'Your Butler Key verification code is: '+ code +''
      });
      return code
}

module.exports.textReminder = function(phoneNumber){
  client.messages.create({
      to: phoneNumber,
      from: process.env.YOUR_TWILIO_NUMBER,
      body: 'BUTLER SERVICE CENTER: The time is 2:30pm, please return your key(s) before 4:30pm at the Service Center.'
    });
    return;
}

module.exports.textLate = function(phoneNumber){
  client.messages.create({
      to: phoneNumber,
      from: process.env.YOUR_TWILIO_NUMBER,
      body: 'BUTLER SERVICE CENTER: The Service Center is now closed. Please return your key(s) to BUPD as soon as possible.'
    });
    return;
}