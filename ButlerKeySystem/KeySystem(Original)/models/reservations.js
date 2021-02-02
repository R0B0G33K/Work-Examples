const Sequelize = require('sequelize');
const db = require('../config/database');

module.exports = db.define(
    'reservations',{
        keyLocation: {
            type: Sequelize.STRING,
            primaryKey: true
        },
        personID:{
            type: Sequelize.INTEGER,
            primaryKey: true
        },
        reservedDate:{
            type: Sequelize.DATE,
            primaryKey: true
        },
        reason: {
            type: Sequelize.STRING
        }
    },
    {
        timestamps: false
    }
);