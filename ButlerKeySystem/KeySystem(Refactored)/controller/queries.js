const Booking = require('../models/bookings');
const Person = require('../models/persons');
const Key = require('../models/keyRings');
const Admin = require('../models/admins');
const Reservation = require('../models/reservations');


//find statments
module.exports = {
    //find all Bookings in DB
    findAllBookings: async () =>{
        let Query = await Booking.findAll({
            raw: true
        })
        .then(bookings =>{
            return bookings;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //finds all People in DB
    findAllPeople: async () =>{
        let Query = await Person.findAll({
            raw: true
        })
        .then(persons =>{
            return persons;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //finds all Admins in DB
    findAllAdmins: async () =>{
        let Query = await Admin.findAll({
            raw: true
        })
        .then(adminss =>{
            return adminss;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //finds all Keys in DB
    findAllKeys: async() =>{
        let Query = await Key.findAll({
            raw: true
        })
        .then(keylist =>{
            return keylist;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //finds all key locations availble (distinct)
    findUniqueKeyLocationsAvailable: async() =>{
        let Query = await Key.aggregate('keyLocation', 'DISTINCT', { 
            plain: false, 
            where: {
                available: true
            }
        })
        .then(keys => {
            var keyLocations = [];
            keys.forEach(function fixFormat(item,index){
                keyLocations[index] = item.DISTINCT;
            })
            return keyLocations;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all people with entered phone number return (ID, company, name, signiture)
    findPersonWithPhoneNum: async(phoneNumber) =>{
        let Query = await Person.findAll({
            raw: true,
            attributes: ['personID','company','name','signiture'],
            where: {
                phoneNum:  phoneNumber
            }
        })
        .then(possiblePerson =>{
            return possiblePerson;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all reservations with ID and date
    findAllReservationsForThisIDAndDate: async(ID,Date) =>{
        let Query = await Reservation.findAll({
            raw: true,
            where: {
                personID:  ID,
                reservedDate: Date
            }
        })
        .then(reservations =>{
            return reservations;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all key numbers available for a location
    findAvailableKeyNumbersForALocation: async (Location) =>{
        let Query = await Key.findAll({
            raw: true,
            attributes: ['keyNum'],
            where: {
                keyLocation:  Location,
                available: true
            }
        })
        .then(openKeys =>{
            return openKeys;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all bookings with this personID, specify attendent should be null
    findAllBookingsByOneID: async (ID, attendentType) =>{
        let Query = await Booking.findAll({
            raw: true,
            attributes: ['keyNum','predictedTime'],
            where: {
                personID:  ID,
                [attendentType]: null
            }
        })
        .then(keysout =>{
            return keysout;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all bookings key numbers by ID and return time
    findAllBookingKeyNumbersByIDAndReturnTime: async (ID, rTime) =>{
        let Query = await Booking.findAll({
            raw: true,
            attributes: ['keyNum'],
            where: {
                personID: ID,
                returnedTime: rTime
            }
        })
        .then(keysin =>{
            return keysin;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find all bookings with no attendent (prep for deletion)
    findAllBookingsWhereAttendentNull: async () =>{
        let Query = await Booking.findAll({
            raw: true,
            attributes: ['keyNum'],
            where: {
                attendent: null
            }
        })
        .then(keysin =>{
            return keysin;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find person name and phone number by ID
    findPersonByID: async (ID) =>{
        let Query =  await Person.findAll({
            raw: true,
            attributes: ['name','phoneNum'],
            where: {
                personID: ID
            }
        })
        .then(OutPerson =>{
            return OutPerson;
        })
        .catch(err => console.log(err));
        return Query;
    },
    //find admin info from email
    findAdminsByEmail: async (email) =>{
        let Query = await Admin.findAll({
            raw: true,
            where: {
              email: email
            }
        })
        .then(loggerIn =>{
            return loggerIn;
        })
        .catch(err => console.log(err));
        return Query;
    }
};

//create statments
module.exports = {
    //add new Email Admin
    createNewAdmin: async(email) =>{
        let Query = await Admin.create({
            email: email,
        }).then( info =>{
            return info;
        });
        return Query;
    },
    //create new Booking
    createBooking: async(keyNumber, ID, pTime, reason, attendent) =>{
        await Booking.create({
            keyNum: keyNumber,
            personID: ID,
            predictedTime: pTime,
            reason: reason,
            attendent: attendent
        });   
    },
    //create new person returns ID of new person
    createPerson: async (name,pNum,comp,sName,sEmail,sNum,signiture) =>{
        const per = await Person.create({
            name: name,
            phoneNum: pNum,
            company: comp,
            sponsName: sName,
            sponsEmail: sEmail,
            sponsPhoneNum: sNum,
            signiture: signiture
        });
        return per.personID;
    },
    //creates a new reservation
    createReservation: async (keyLocation, ID, rDate, reason) =>{
        await Reservation.create({
            keyLocation: keyLocation,
            personID: ID,
            reservedDate: rDate,
            reason: reason,
        });
    }
};

//update statments
module.exports = {
    //update availibility of key by key number
    updateKeyAvailability: async (availibility,keyNumber) =>{
        await Key.update(
            {
                available: availibility
            },
            {
            where: {
                keyNum: keyNumber
            }
        }
        ).then(function() {
            console.log('Key '+currentKeyNum+' has been updated!')
        }).catch(err => console.log(err));
    },
    //update bookings on return
    updateBookingsOnReturn: async (rTime,attendent,ID) =>{
        await Booking.update(
            {
                returnedTime: rTime,
                attendent2: attendent
            },
            {
            where: {
                personID:  ID,
                attendent2: null
            }
        }
        ).then(function() {
            console.log('added sign out attendent');
        }).catch(err => console.log(err));
    },
    //update booking on reservation checkout
    updateBookingsOnReservationCheckout: async (attendent, pTime, ID) =>{
        await Booking.update(
            {
                attendent: attendent,
                predictedTime: pTime
            },
            {
            where: {
                personID:  ID,
                attendent: null,
                predictedTime: null
            }
        }
        ).then(function() {
            console.log('added sign in attendent and predicted time');
        }).catch(err => console.log(err));
    },
    //Update all bookings by ID sign out attendents
    updateBookingsSignOutAttendent: async (attendent, ID) =>{
        await Booking.update(
            {
                attendent: attendent
            },
            {
            where: {
                personID:  ID,
                attendent: null
            }
        }
        ).then(function() {
            console.log('added sign in attendent');
        }).catch(err => console.log(err));
    },
    //Update person info with ID, return ID again
    updatePersonByID: async (ID,name,pNum,comp,sName,sEmail,sNum,signiture) =>{
        let Query = await Person.update(
            {
                name: name,
                phoneNum: pNum,
                company: comp,
                sponsName: sName,
                sponsEmail: sEmail,
                sponsPhoneNum: sNum,
                signiture: signiture
            },
            {
            where: {
                personID: ID
            }
        }
        ).then(function() {
            console.log('updated person #'+ID+'');
            return ID;
        }).catch(err => console.log(err));
        return Query;
    }
};

//destroy statments
module.exports = {
    //destroy reservation
    destroyReservation: async (Location,ID,rDate) =>{
        await Reservation.destroy({
            where: {
                keyLocation: Location,
                personID: ID,
                reservedDate: rDate
            }
        })
    },
    //destroy timed out bookings
    destroyBookingsWithNoAttendent: async () =>{
        await Booking.destroy({
            where: {
                attendent: null
            }
        });
    },
    //destroy reservation when expired
    destroyReservationWhenOutDate: async (date) =>{
        await Reservation.destroy({
            where: {
                reservedDate: date
            }
        });
    }
};

//aggregation
module.exports = {
    bookingsNotRetuned: async () =>{
        let Query = await Booking.aggregate('personID', 'DISTINCT', { plain: false, raw: true, where:{returnedTime: null}}
        ).then(bookings =>{
            return bookings;
        }).catch(err => console.log(err));
        return Query;
    }
};