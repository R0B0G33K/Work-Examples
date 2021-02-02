const Sequelize = require('sequelize');
const db = require('../config/database');

module.exports = db.define(
    'persons',{
        personID: {
            type: Sequelize.INTEGER,
            autoIncrement: true,
            primaryKey: true
        },
        name:{
            type: Sequelize.STRING,
            allowNull: false
        },
        phoneNum:{
            type: Sequelize.STRING,
            primaryKey: true
        },
        company:{
            type: Sequelize.STRING
        },
        sponsName:{
            type: Sequelize.STRING
        },
        sponsEmail:{
            type: Sequelize.STRING
        },
        sponsPhoneNum:{
            type: Sequelize.STRING
        },
        signiture:{
            type: Sequelize.BLOB
        }
    },
    {
        timestamps: false
    }
);